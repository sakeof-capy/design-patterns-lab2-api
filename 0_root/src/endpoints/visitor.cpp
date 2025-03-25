#include "endpoints/visitor.hpp"

#include <format>
#include <vector>
#include <nlohmann/json.hpp>

namespace json = nlohmann;

struct Visitor 
{
    std::string name;
    int id;
    int age;
};

void to_json(json::json& j, const Visitor& v) 
{
    j = json::json{
        {"id", v.id},
        {"name", v.name},
        {"age", v.age}
    };
}

std::vector<Visitor> generate_visitors()
{
    std::vector<Visitor> visitors;
    const int num_visitors = rand() % 10 + 1;

    for (int i = 0; i < num_visitors; ++i) 
    {
        visitors.push_back(Visitor {
            .name = std::format("Capy McCapface{}", i),
            .id = i,
            .age = i * 10 + 22,
        });
    }

    return visitors;
}

void visitor_endpoints_check_in(httplib::Server& server)
{
    server.Get("/visitors", [](const httplib::Request&, httplib::Response& res) {
        res.set_content_provider("text/event-stream", [&](size_t offset, httplib::DataSink& sink) {
            res.set_header("Content-Type", "text/event-stream");
            res.set_header("Cache-Control", "no-cache");
            res.set_header("Connection", "keep-alive");
    
            while (true) 
            {
                const std::vector<Visitor> visitors = generate_visitors();
                json::json visitors_json = json::json::array();
    
                for (const Visitor& visitor : visitors) 
                {
                    visitors_json.push_back(visitor);
                }
    
                const std::string message = "data: " + visitors_json.dump() + "\n\n";
                sink.write(message.data(), message.size());
    
                std::this_thread::sleep_for(std::chrono::seconds(3));
            }
    
            return true;
        });
    });
}
