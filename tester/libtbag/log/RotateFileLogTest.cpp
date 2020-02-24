/**
 * @file   RotateFileLogTest.cpp
 * @brief  RotateFile logger tester.
 * @author zer0
 * @date   2020-02-24
 */

#include <gtest/gtest.h>
#include <libtbag/log/Log.hpp>
#include <libtbag/log/sink/RotateFileSink.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <libtbag/string/Format.hpp>
#include <libtbag/Unit.hpp>
#include <libtbag/archive/Archive.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::sink;

TEST(RotateFileLogTest, Default)
{
    char const * const TEST_FILENAME = "file.log";
    tttDir_Automatic();
    auto const dir = tttDir_Get();
    auto const rotate_mega_byte = 1;
    auto const * archive = ".zip";

    using namespace libtbag::string;
    auto const arguments = fformat("size={}m archive={} time={}/log-$py$pm$pdT$ph$pi$ps_$pl$pc.log",
                                   rotate_mega_byte, archive, dir.toString());

    LoggerInitParams init;
    init.name = std::string(test_info_->test_case_name()) + test_info_->name();
    init.sink = RotateFileSink::sink_name();
    init.arguments = arguments;
    init.generator = GENERATOR_RAW;
    init.line_feed = "";
    init.severity = INFO_SEVERITY.getText();
    init.auto_flush = "true";
    init.thread = "";

    auto const * logger = createLogger(init);
    ASSERT_NE(nullptr, logger);
    ASSERT_TRUE(dir.scanDir().empty());

    auto const write_message_byte = 1024;
    auto const log_message = std::string(write_message_byte, '0');
    auto const total_log_size = rotate_mega_byte * MEGA_BYTE_TO_BYTE;
    auto const loop_count = total_log_size / write_message_byte;
    for (auto i = 0; i < loop_count-1; ++i) {
        tLogI(init.name, log_message);
    }
    auto const files0 = dir.scanDir();
    ASSERT_EQ(1, files0.size()); // log file.
    auto const first_log_path = files0[0];
    ASSERT_STREQ(".log", first_log_path.getExtensionName().c_str());

    tLogI(init.name, log_message);
    auto files1 = dir.scanDir();
    using namespace libtbag::filesystem;
    std::sort(files1.begin(), files1.end(), [](Path const & p1, Path const & p2){
        return p1.toString() < p2.toString();
    });
    ASSERT_EQ(2, files1.size()); // Zip file, and next log file.
    ASSERT_FALSE(first_log_path.exists());

    auto const zip_path = files1[0];
    auto const next_log_path = files1[1];
    ASSERT_STREQ(".log.zip", zip_path.getExtensionName().c_str());
    ASSERT_STREQ(".log", next_log_path.getExtensionName().c_str());
    std::cout << "Zip file: " << zip_path << std::endl;
    std::cout << "Next log file: " << next_log_path << std::endl;

    auto const entries = libtbag::archive::decompressArchive(zip_path);
    ASSERT_EQ(1, entries.size());
    ASSERT_EQ(total_log_size, entries[0].data.size());
    ASSERT_EQ('0', entries[0].data[0]);
    ASSERT_EQ('0', entries[0].data[entries[0].data.size()-1]);

    ASSERT_TRUE(removeLogger(logger));
}

