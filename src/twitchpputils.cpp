#include "twitchpputils.hpp"

namespace TwitchPP {
    Response<std::string> call_api(std::string_view url,
                                   std::string_view bearer,
                                   std::string_view client_id,
                                   std::optional<std::string_view> request_type,
                                   std::optional<std::string> request_body) {
        std::string result {""};
        std::vector<std::string> headers {};
        size_t response_code {500};
        CURL *curl;
        CURLcode res;
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        if (curl) {
            struct curl_slist *chunk = nullptr;
            chunk = curl_slist_append(chunk, static_cast<std::string>("Authorization: Bearer " ).append(bearer.data()).data());
            chunk = curl_slist_append(chunk, static_cast<std::string>("Client-Id: ").append(client_id.data()).data());
            if (request_body) {
                chunk = curl_slist_append(chunk, "Content-Type: application/json");
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_body.value().c_str());
            }
            if (request_type) {
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request_type.value().data());
            }
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, resp_size_cb);
            curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, resp_header_cb);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &headers);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
            curl_easy_setopt(curl, CURLOPT_URL, url.data());

            res = curl_easy_perform(curl);
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            // TODO: for some reason seg fault in wrong OAUTH

            curl_slist_free_all(chunk);
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
        if (res != CURLE_OK) {
            std::cout << "Error: " << curl_easy_strerror(res) << std::endl;
            return {"", "", response_code, curl_easy_strerror(res), headers};
        }
        return {result, "", response_code, "", headers};
    }

    size_t resp_size_cb(char *ptr, size_t size, size_t nmemb, void *stream) {
        static_cast<std::string*>(stream)->append(static_cast<char*>(ptr));
        return size * nmemb;
    }

    size_t resp_header_cb(char *buffer, size_t size, size_t nitems, void *userdata) {
        static_cast<std::vector<std::string>*>(userdata)->push_back(std::string(buffer));
        return size * nitems;
    }

    std::string generate_oauth_url(std::string_view client_id, std::string_view redirect_url, std::vector<std::string_view> scope) {
        std::string url {"https://id.twitch.tv/oauth2/authorize?client_id="};
        url.append(client_id.data()).append("&redirect_uri=").append(redirect_url.data()).append("&response_type=token&scope=");
        for (size_t i {0}; i < scope.size(); i++) {
            url.append(scope[i].data());
            if (i < scope.size() - 1) {
                url.append("+");
            }
        }
        return url;
    }

    size_t get_dqs(std::string_view elements, const size_t& starting_pos, const size_t& ending_pos) {
        size_t dq_counter {0};
        for (size_t i {starting_pos}; i <= ending_pos; ++i) {
            if (elements.at(i) == '"') {
                if (i > 0 && elements.at(i - 1) == '\\') {
                    continue;
                }
                dq_counter++;
            }
        }
        return dq_counter;
    }

    std::string get_object_param(std::string_view param_name,
                                 std::string_view obj_string,
                                 std::optional<std::string_view> fallback) {
        size_t tmp_first_pos {0};
        size_t tmp_second_pos {std::string::npos};
        if (!obj_string.size()) {
            return std::string(fallback.value_or(""));
        }
        size_t tmp_start_pos {obj_string.find(param_name)};
        if (tmp_start_pos != std::string::npos) {
            char opening_char {obj_string[tmp_start_pos + param_name.size() + 1]}, closing_char {0};
            try {
                closing_char = closing_chars.at(opening_char);
            }
            catch(const std::out_of_range& e) {}
            if (closing_char) {
                tmp_start_pos += param_name.size() + 2;
                tmp_first_pos = tmp_start_pos;
                bool found {false};
                size_t checkpoint = obj_string.find(opening_char, tmp_first_pos);
                while (!found) {
                    tmp_second_pos = obj_string.find(closing_char, tmp_first_pos);
                    if (tmp_second_pos == std::string::npos) {
                        found = true;
                        return std::string(fallback.value_or(""));
                    } else if (obj_string[tmp_second_pos - 1] == '\\') {
                        tmp_first_pos = tmp_second_pos + 1;
                    } else if(checkpoint != std::string::npos && checkpoint < tmp_second_pos && obj_string[checkpoint - 1] != '\\') {
                        tmp_first_pos = tmp_second_pos + 1;
                        checkpoint = obj_string.find(opening_char, checkpoint + 1);
                    } else if(get_dqs(obj_string, tmp_start_pos - 1, tmp_second_pos) % 2) {
                        tmp_first_pos = tmp_second_pos + 1;
                    } else {
                        found = true;
                    }
                }
            } else {
                tmp_start_pos += param_name.size() + 1;
                for (char closing_separator : ",}") {
                    tmp_second_pos = obj_string.find(closing_separator, tmp_start_pos);
                    if (tmp_second_pos != std::string::npos) {
                        break;
                    } else if (closing_separator == '}') {
                        tmp_second_pos = obj_string.size();
                    }
                }
            }
        } else {
            return std::string(fallback.value_or(""));
        }
        return static_cast<std::string>(obj_string.substr(tmp_start_pos, tmp_second_pos - tmp_start_pos));
    }

    std::pair<std::string, std::string> get_first_value(std::string initial_string) {
        size_t t = initial_string.find_first_of("{[");
        size_t starting_pos {t};
        size_t start_count {1};
        size_t end_count {0};
        if (t != std::string::npos) {
            while (start_count != end_count && t != std::string::npos) {
                t = initial_string.find_first_of("{[]}", t + 1);
                if (t == std::string::npos) {
                    continue;
                }
                if (initial_string[t] == '{' || initial_string[t] == '[') {
                    start_count++;
                } else {
                    if (get_dqs(initial_string, starting_pos, t) % 2) {
                        continue;
                    }
                    end_count++;
                }
            }
        }
        if (start_count != end_count && t == std::string::npos) {
            return std::pair<std::string, std::string>("", "");
        }
        return std::pair<std::string, std::string>(initial_string.substr(starting_pos + 1, t - starting_pos - 1), initial_string.substr(t, initial_string.size()));
    }

    std::string replace_string(std::string subject, std::string_view search, std::string_view replace) {
        for (size_t pos {subject.find(search, 0)}; pos < std::string::npos; pos = subject.find(search, pos)) {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
        return subject;
    }

    std::string vector_to_json(const std::vector<std::string>& elements) {
        std::string result {"["};
        for (size_t i {0}; i < elements.size(); ++i) {
            result += "\"" + elements[i] + "\"" + (i + 1 < elements.size() ? "," : "");
        }
        return result + "]";
    }

    std::vector<std::string> json_to_vector(std::string_view elements) {
        std::vector<std::string> result {};
        size_t end_pos {std::string::npos};
        char opening_char {elements[0]}, closing_char {static_cast<char>('"')};
        size_t checkpoint {std::string::npos};
        try {
            closing_char = closing_chars.at(opening_char);
        }
        catch(const std::out_of_range& e) {}
        for (size_t start_pos {0}; start_pos < elements.size(); start_pos = elements.find(std::string(",") + opening_char, start_pos)) {
            if (start_pos > 0 && start_pos < std::string::npos) {
                start_pos += 1;
            }
            end_pos = elements.find(closing_char, start_pos + 1);
            checkpoint = elements.find(opening_char, start_pos + 2);
            while (checkpoint < end_pos){
                checkpoint = elements.find(opening_char, checkpoint + 1);
                end_pos = elements.find(closing_char, end_pos + 1);
            }
            int diff = elements[start_pos] == opening_char ? 1 : 2;
            result.push_back(std::string(elements.substr(start_pos + diff, end_pos - start_pos - diff)));
            start_pos = end_pos;
        }
        return result;
    }

    StringMap string_to_string_map(std::string_view elements) {
        std::vector<std::string> entries = json_to_vector(elements);
        StringMap result {};
        for (size_t start_pos {0}, end_pos {0}; start_pos < elements.size() - 1; start_pos = end_pos + 1) {
            end_pos = elements.find("\":\"", start_pos + 1);
            std::string str_key {elements.substr(start_pos + 1, end_pos - start_pos - 1)};
            start_pos = end_pos + 3;
            end_pos = elements.find(",\"", start_pos);
            if (end_pos == std::string::npos) {
                end_pos = elements.size();
            }
            std::string str_val {elements.substr(start_pos, end_pos - start_pos - 1)};
            result[str_key] = str_val;
        }
        return result;
    }

    std::string string_map_to_string(const StringMap& elements) {
        std::string result {""};
        for (const auto& x : elements) {
            result +=  "\"" + x.first + "\":\"" + x.second + "\",";
        }
        return result.size() ? result.substr(0, result.size() - 1) : "";
    }
}
