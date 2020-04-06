#include "debug.hpp"
// Как использовать
//	src::severity_logger<severity_level> lg;
// LOGGER(lg, trace) << "trace";
//


template<typename ValueType>
ValueType set_get_attrib(const char* name, ValueType value) {
   auto attr = logging::attribute_cast<attrs::mutable_constant<ValueType>>(logging::core::get()->get_thread_attributes()[name]);
   attr.set(value);
   return attr.get();
}


// Convert file path to only the filename
std::string path_to_filename(std::string path) {
   return path.substr(path.find_last_of("/\\")+1);
}


void debugInit(){
    boost::log::add_common_attributes();
//    boost::log::core::get()->add_global_attribute("Scope", 
  //      boost::log::attributes::named_scope());


   logging::core::get()->add_thread_attribute("File", attrs::mutable_constant<std::string>(""));
   logging::core::get()->add_thread_attribute("Line", attrs::mutable_constant<int>(0));

    auto fmtTimeStamp = boost::log::expressions::
        format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f");

	auto fmtThreadId = boost::log::expressions::
        attr<boost::log::attributes::current_thread_id::value_type>("ThreadID");

	auto fmtSeverity = boost::log::expressions::
        attr<boost::log::trivial::severity_level>("Severity");


	auto fmtFile = boost::log::expressions::attr<std::string>("File");
	auto fmtLine = boost::log::expressions::attr<int>("Line");

	//	auto fmtScope = boost::log::expressions::format_named_scope("Scope",
  //      boost::log::keywords::format = "%n(%F:%l)",
    //    boost::log::keywords::iteration = boost::log::expressions::reverse,
      //  boost::log::keywords::depth = 2);

	boost::log::formatter logFmt =
        boost::log::expressions::format("[%1%] (%2%) [%3%] [%4%:%5%] %6%")
        % fmtTimeStamp % fmtThreadId % fmtSeverity
		%fmtFile %fmtLine
        % boost::log::expressions::smessage ;

    /* console sink */
    auto consoleSink = boost::log::add_console_log(std::clog);
    consoleSink->set_formatter(logFmt);

	src::severity_logger<severity_level> lg;
	LOGGER(lg, warning) << "Logger template generation function";
}
