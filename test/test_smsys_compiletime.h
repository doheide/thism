//
// MIT License
//
// Copyright (c) 2018 Dominik Heide
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include <QString>

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <thism/sm2.h>
#include <thism/sm2_test_helper.h>

#include <base_sys/base_test_sys.h>

#include <QResource>
#include <QFile>
#include <QTextStream>


#define NESTED_COMPILATION_BASE "../../thism/"


using namespace testing;


struct CompileTimeTest : public ::testing::TestWithParam<std::pair<const char*, const char*>> {

    CompileTimeTest()
    { }

protected:
    void SetUp() override {}
    void TearDown() override {}

    int compileFile(const char *test_file) {
        Q_INIT_RESOURCE(ctt);

        //QByteArray testCode;
        QString testCode;
        {
            QFile file(QString(":/ctt/compile_time_tests/%1").arg(test_file));
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                return -1;
            testCode.append(file.readAll());
        }
        {
            QFile file(test_file);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return -1;
            QTextStream ts(&file);
            ts << testCode;
        }


        QString cmd = QString("gcc -c -I%1/lib/include/ -I%1/test/base_sys/ -DBAHA_TYPE=BAHA_Void -DBAHA_INCLUDE=baha_base.h %2 2> compile_output.log")
                .arg(NESTED_COMPILATION_BASE).arg(test_file);
        int r = std::system(cmd.toStdString().c_str());

        return r;
    }
};

TEST_P(CompileTimeTest, compile_expected_to_fail) {

    // ASSERT_THAT(GetParam(), HasSubstr("ny"));

    auto param = GetParam();
    const char *cfile = param.first;
    int r = compileFile(cfile);
    ASSERT_NE(r, 0) << "Test case should not compile but '" << cfile << "' seems to compile.";

    QFile file("compile_output.log");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        FAIL() << "compile_output.log for building " << cfile << " not found.";
    QString s; s.append(file.readAll());

    EXPECT_NE(s.indexOf("CTC"), -1) << "Compilation error but it does not seem to be an ASSERTION for file '" << cfile << ":\n" << s.toStdString() << "'";
    EXPECT_NE(s.indexOf(param.second), -1) << "Compilation error but the expected response is not present in file '" << cfile << ":\n" << s.toStdString() << "'";

}

INSTANTIATE_TEST_CASE_P(InstantiationName, CompileTimeTest,
                        ::testing::Values(std::make_pair("test_event.cpp", "Event struct has to be have"),
                                          std::make_pair("test_state_invalid_type.cpp", "State struct has to have a member"),
                                          std::make_pair("test_state_no_initial.cpp", "No initial state defined."),
                                          std::make_pair("test_event_missing.cpp", "Transition event is not an event of the parent-system"),
                                          std::make_pair("test_state_missing.cpp", "Transition destination is not a state of the parent-statemachine"),
                                          std::make_pair("test_state_duplicate_in_sm.cpp", "No duplicates are allowed in state list of state machine"),
                                          std::make_pair("test_state_duplicate_in_sys.cpp", "No duplicates are allowed in state list of all state machine in the system") ));



