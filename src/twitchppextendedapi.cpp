#include "twitchppextendedapi.hpp"

namespace TwitchPP {
    TwitchExtendedAPI::TwitchExtendedAPI(const std::string& app_access_token,
                                         const std::string& client_id)
                                         : TwitchAPI{app_access_token, client_id} {
    }

    VectorResponse<TwitchEventSubSubscriptions> TwitchExtendedAPI::get_eventsub_subscriptions(std::optional<std::string_view> filter,
                                                                                              bool is_type,
                                                                                              std::optional<std::string> after) {
        std::string options {"?"};
        if (filter) {
            options += (is_type ? "type=" : "status=") + std::string(filter.value());
        }
        if (after) {
            options += (options == "?" ? "after=" : "&after=") + after.value();
        }
        std::string url {TWITCH_API_BASE + "eventsub/subscriptions" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        VectorResponseLeftovers<TwitchEventSubSubscription> subs_response = this->process_response_leftovers<TwitchEventSubSubscription>(response);
        size_t total = std::stoul(get_object_param("\"total\"", response.data, "0"));
        size_t total_cost = std::stoul(get_object_param("\"total_cost\"", subs_response.leftovers, "0"));
        size_t max_total_cost = std::stoul(get_object_param("\"max_total_cost\"", subs_response.leftovers, "0"));
        return {{TwitchEventSubSubscriptions(total, total_cost, max_total_cost, subs_response.data)}, response.cursor, response.code, response.message};
    }
    // TODO: Might refactor both methods to avoid code duplication
    VectorResponse<TwitchEventSubSubscriptions> TwitchExtendedAPI::create_eventsub_subscription(std::string_view type,
                                                                                                std::string_view version,
                                                                                                TwitchCondition condition,
                                                                                                TwitchTransport transport) {
        std::string request_body {"{\"type\":\"" + std::string(type)
            + "\",\"version\":\"" + std::string(version)
            + "\",\"condition\":" + condition.to_json()
            + ",\"transport\":" + transport.to_json() + "}"};
        std::string url {TWITCH_API_BASE + "eventsub/subscriptions"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_POST, request_body);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        VectorResponseLeftovers<TwitchEventSubSubscription> subs_response = this->process_response_leftovers<TwitchEventSubSubscription>(response);
        size_t total = std::stoul(get_object_param("\"total\"", subs_response.leftovers, "0"));
        size_t total_cost = std::stoul(get_object_param("\"total_cost\"", subs_response.leftovers, "0"));
        size_t max_total_cost = std::stoul(get_object_param("\"max_total_cost\"", subs_response.leftovers, "0"));
        return {{TwitchEventSubSubscriptions(total, total_cost, max_total_cost, subs_response.data)}, response.cursor, response.code, response.message};
    }

    VectorResponse<TwitchEventSubSubscriptions> TwitchExtendedAPI::create_eventsub_subscription(std::string_view type,
                                                                                                std::string_view version,
                                                                                                std::string_view condition,
                                                                                                std::string_view transport) {
        std::string request_body {"{\"type\":\"" + std::string(type)
            + "\",\"version\":\"" + std::string(version)
            + "\",\"condition\":" + std::string(condition)
            + ",\"transport\":" + std::string(transport) + "}"};
        std::string url {TWITCH_API_BASE + "eventsub/subscriptions"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_POST, request_body);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        std::cout << response.data << std::endl;
        VectorResponseLeftovers<TwitchEventSubSubscription> subs_response = this->process_response_leftovers<TwitchEventSubSubscription>(response);
        size_t total = std::stoul(get_object_param("\"total\"", subs_response.leftovers, "0"));
        size_t total_cost = std::stoul(get_object_param("\"total_cost\"", subs_response.leftovers, "0"));
        size_t max_total_cost = std::stoul(get_object_param("\"max_total_cost\"", subs_response.leftovers, "0"));
        return {{TwitchEventSubSubscriptions(total, total_cost, max_total_cost, subs_response.data)}, response.cursor, response.code, response.message};
    }
}
