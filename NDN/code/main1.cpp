#include <stdio.h>
#include <stdlib.h>
#include "httplib.h"
#include <json/json.h>
#include <future>

static int requestCounter = 0;
struct MyData {
  std::string id;
  std::string name;
  // 添加其他字段...

  // 打印结构体中的数据
  void printData() {
    std::cout << "Name: " << id << std::endl;
    std::cout << "Data: " << name << std::endl;
    // 打印其他字段...
  }
};
void save_to_file(const std::string& filename, const std::string& content) {
  std::ofstream file(filename, std::ios::trunc);  // 打开文件，如果存在则截断文件
  if (file.is_open()) {
    file << content;
    file.close();
  } else {
    std::cerr << "Unable to open file: " << filename << std::endl;
  }
}
std::string read_from_file(const std::string& filename) {
  std::ifstream file(filename);
  if (file.is_open()) {
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
  } else {
    std::cerr << "Unable to open file: " << filename << std::endl;
    return "";
  }
}
void handle_request(const httplib::Request& req, httplib::Response& res) {
  // 解析 JSON 数据
  requestCounter++;
  Json::Value jsonData;
  Json::CharReaderBuilder builder;
  std::istringstream jsonStream(req.body);
  Json::parseFromStream(builder, jsonStream, &jsonData, nullptr);

  MyData myData;
  myData.id = jsonData["name"].asString();
  myData.name = jsonData["data"].asString();
  myData.printData();
//  调用 ndnpoke，并将解析后的数据传递给它
  res.status = 200;  // 设置 HTTP 响应状态码为 200（OK）
  res.set_content("Data received successfully", "text/plain"); 
  std::string filename = "ien" + std::to_string(requestCounter) + ".txt";
  save_to_file(filename, req.body);
  // std::string ndnpokeCommand = "echo '" + req.body + "' | ndnpoke /ien";
  // std::system(ndnpokeCommand.c_str());
  //std::cout << "Executing command: " << ndnpokeCommand << std::endl;
  //   // 处理 ndnpoke 的执行结果
   // std::async(std::launch::async, [req_body = req.body] {
   // std::string ndnpokeCommand = "echo '" + req_body + "' | ndnpoke /ien"+std::to_string(requestCounter);
   // std::system(ndnpokeCommand.c_str());
   // std::cout << "Executing command: " << ndnpokeCommand << std::endl;
   // });  
   // 转换为时间戳
  auto currentTimePoint = std::chrono::system_clock::now();
   auto timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
        currentTimePoint.time_since_epoch()
    ).count();
   auto timestamp1 = std::chrono::duration_cast<std::chrono::seconds>(
        currentTimePoint.time_since_epoch()
    ).count();
    // 打印时间戳
        std::cout << "Current timestamp(s): " << timestamp1 << std::endl;

    std::cout << "Current timestamp: " << timestamp << std::endl;
}
void doGetHi(const httplib::Request& req, httplib::Response& res)
{
	std::cout << "ID:" << std::endl;
    res.set_content("hi", "text/plain");
}
void handle_get_request(const httplib::Request& req, httplib::Response& res) {
  std::string filename = req.get_param_value("filename");
  if (std::ifstream(filename)) {
    std::string content = read_from_file(filename);
    res.status = 200;
    res.set_content(content, "text/plain");
  } else {
    res.status = 404;
    res.set_content("File not found", "text/plain");
  }
}
int main(int argc, char *argv[])
{
    httplib::Server server;
    server.Get("/hi", doGetHi);
   server.Post("/peekData",handle_request);
  server.Get("/getfile", handle_get_request); // 新增的获取文件内容的接口
    server.listen("0.0.0.0", 8000);
    return 0;
}
