#include <curl/curl.h>
#include <string>
#include <string_view>
#include <iostream>
#include <vector>
#include <optional>
#include <map>
#include <memory>

namespace TwitchPP {
    template<typename T> struct Response {
        T data;
        std::string cursor;
        size_t code;
        std::string message;
    };

    struct VideoSegment {
        size_t duration;
        size_t offset;
    };

    static std::map<char, char> closing_chars { {'\"', '\"'}, {'{', '}'}, {'[', ']'} };

    size_t resp_size_cb(char *ptr, size_t size, size_t nmemb, void *stream);
    Response<std::string> call_api(std::string_view url,
                                   std::string_view bearer,
                                   std::string_view client_id,
                                   std::optional<std::string_view> request_type = std::nullopt,
                                   std::optional<std::string> request_body = std::nullopt);
    std::string generate_oauth_url(std::string_view client_id, std::string_view redirect_url, std::vector<std::string_view> scope);
    std::pair<std::string, std::string> get_first_value(std::string initial_string);
    std::string get_object_param(std::string_view param_name, std::string_view obj_string, std::optional<std::string_view> fallback = std::nullopt, size_t tmp_first_pos=0, size_t tmp_second_pos=std::string::npos);
    std::string replace_string(std::string subject, std::string_view search, std::string_view replace);
    std::string vector_to_json(std::vector<std::string>& elements);
    std::vector<std::string> json_to_vector(std::string_view elements);
    std::vector<VideoSegment> json_to_segment_vector(std::string_view elements);
}
