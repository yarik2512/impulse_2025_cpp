#include <gtest/gtest.h>
#include "../src/chronoio.h"
#include "../src/action.h"
#include "../src/club.h"

TEST(chronoio, valid_input) {
    const std::vector<std::string> inputs = {
        "10:00",
        "00:59",
        "23:59",
    };
    std::chrono::time_point<std::chrono::system_clock> t;
    for (const std::string &input: inputs) {
        std::stringstream in(input);
        EXPECT_NO_THROW({ in >> t; }) << "INPUT: " << input;
    }
}

TEST(chronoio, invalid_input) {
    const std::vector<std::string> inputs = {
        "abacaba",
        "-1:43",
        "00:-1",
        "31:05",
        "24:00",
        "23:61"
    };
    std::chrono::time_point<std::chrono::system_clock> t;
    for (const std::string &input: inputs) {
        std::stringstream in(input);
        EXPECT_THROW({ in >> t; }, std::runtime_error) << "INPUT: " << input;
    }
}

TEST(action, valid_input) {
    const std::vector<std::string> inputs = {
        "12:00 1 client",
        "12:00 2 client 1",
        ""
    };
    Action a;
    for (const std::string &input: inputs) {
        std::stringstream in(input);
        EXPECT_NO_THROW({ in >> a; }) << "INPUT: " << input;
    }
}

TEST(action, invalid_input) {
    const std::vector<std::string> inputs = {
        "10:61 1 client",
        "12:61 2 client",
        "abacaba",
        "12:00 1 client 1",
        "12:00 1",
        "12:00"
    };
    Action a;
    for (const std::string &input: inputs) {
        std::stringstream in(input);
        EXPECT_THROW({ in >> a; }, std::runtime_error) << "INPUT: " << input;
    }
}

TEST(club, valid_input) {
    const std::vector<std::string> inputs = {
        "1\n12:00 19:00\n100"
    };
    Club c;
    for (const std::string &input: inputs) {
        std::stringstream in(input);
        EXPECT_NO_THROW({ in >> c; }) << "INPUT: " << input;
    }
}

TEST(club, invalid_input) {
    const std::vector<std::string> inputs = {
        "0\n12:00 19:00\n100",
        "1\n12:61 19:00\n100",
        "1\n12:00 19:00\n-1",
        "1 2\n12:00 19:00\n100",
        "1\n12:00 19:00 1\n100",
        "1\n12:00 19:00\n100 1",
        "1\n19:00 12:00\n100",
    };
    Club c;
    for (const std::string &input: inputs) {
        std::stringstream in(input);
        EXPECT_THROW({ in >> c; }, std::runtime_error) << "INPUT: " << input;
    }
}

TEST(club, process_action) {
    Club c;
    const std::string input(
        "1\n"
        "10:00 20:00\n"
        "10\n"
    );
    std::stringstream in(input);
    in >> c;
    Club::time_point tp;
    std::stringstream tpin("11:00");
    tpin >> tp;
    try {
        c.process_action({tp, 2, "client", 1});
        FAIL() << "Expected exception of type Action";
    } catch (const Action &a) {
        EXPECT_EQ(a.time, tp);
        EXPECT_EQ(a.id, 13);
        EXPECT_EQ(a.client, "ClientUnknown");
    }
    try {
        c.process_action({tp, 3, "client"});
        FAIL() << "Expected exception of type Action";
    } catch (const Action &a) {
        EXPECT_EQ(a.time, tp);
        EXPECT_EQ(a.id, 13);
        EXPECT_EQ(a.client, "ICanWaitNoLonger");
    }
    try {
        c.process_action({tp, 4, "client"});
        FAIL() << "Expected exception of type Action";
    } catch (const Action &a) {
        EXPECT_EQ(a.time, tp);
        EXPECT_EQ(a.id, 13);
        EXPECT_EQ(a.client, "ClientUnknown");
    }
    ASSERT_NO_THROW(c.process_action({tp, 1, "client"}));
    try {
        c.process_action({tp, 1, "client"});
        FAIL() << "Expected exception of type Action";
    } catch (const Action &a) {
        EXPECT_EQ(a.time, tp);
        EXPECT_EQ(a.id, 13);
        EXPECT_EQ(a.client, "YouShallNotPass");
    }
    using namespace std::chrono_literals;
    try {
        c.process_action({c.get_open_time() - 1min, 1, "client2"});
        FAIL() << "Expected exception of type Action";
    } catch (const Action &a) {
        EXPECT_EQ(a.time, c.get_open_time() - 1min);
        EXPECT_EQ(a.id, 13);
        EXPECT_EQ(a.client, "NotOpenYet");
    }
    try {
        c.process_action({c.get_close_time() + 1min, 1, "client2"});
        FAIL() << "Expected exception of type Action";
    } catch (const Action &a) {
        EXPECT_EQ(a.time, c.get_close_time() + 1min);
        EXPECT_EQ(a.id, 13);
        EXPECT_EQ(a.client, "NotOpenYet");
    }
    ASSERT_NO_THROW(c.process_action({tp, 2, "client", 1}));
    ASSERT_NO_THROW(c.process_action({tp, 1, "client2"}));
    try {
        c.process_action({tp, 2, "client2", 1});
        FAIL() << "Expected exception of type Action";
    } catch (const Action &a) {
        EXPECT_EQ(a.time, tp);
        EXPECT_EQ(a.id, 13);
        EXPECT_EQ(a.client, "PlaceIsBusy");
    }
    ASSERT_NO_THROW(c.process_action({tp, 3, "client2"}));
    ASSERT_NO_THROW(c.process_action({tp, 1, "client3"}));
    try {
        c.process_action({tp, 3, "client3"});
        FAIL() << "Expected exception of type Action";
    } catch (const Action &a) {
        EXPECT_EQ(a.time, tp);
        EXPECT_EQ(a.id, 11);
        EXPECT_EQ(a.client, "client3");
    }
    try {
        c.process_action({tp, 4, "client"});
        FAIL() << "Expected exception of type Action";
    } catch (const Action &a) {
        EXPECT_EQ(a.time, tp);
        EXPECT_EQ(a.id, 12);
        EXPECT_EQ(a.client, "client2");
    }
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
