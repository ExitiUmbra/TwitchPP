#include "twitchppextendedapi.hpp"

namespace TwitchPP {
    TwitchExtendedAPI::TwitchExtendedAPI(const std::string& app_access_token,
                                         const std::string& client_id)
                                         : TwitchAPI{app_access_token, client_id} {
    }

    VectorResponse<TwitchEventSubSubscriptions> TwitchExtendedAPI::get_eventsub_subscriptions(std::optional<std::string_view> filter,
                                                                                              const bool& is_type,
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
        VectorResponseLeftovers<TwitchEventSubSubscription> subs_response = this->process_response_leftovers<TwitchEventSubSubscription>(response);
        size_t total = std::stoul(get_object_param("\"total\"", subs_response.leftovers, "0"));
        size_t total_cost = std::stoul(get_object_param("\"total_cost\"", subs_response.leftovers, "0"));
        size_t max_total_cost = std::stoul(get_object_param("\"max_total_cost\"", subs_response.leftovers, "0"));
        return {{TwitchEventSubSubscriptions(total, total_cost, max_total_cost, subs_response.data)}, response.cursor, response.code, response.message};
    }

    Response<std::string> TwitchExtendedAPI::delete_eventsub_subscription(std::string_view subscription_id) {
        std::string options {"?id=" + std::string(subscription_id)};
        std::string url {TWITCH_API_BASE + "eventsub/subscriptions" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_DELETE);
        return response;
    }

    VectorResponse<TwitchCodeStatus> TwitchExtendedAPI::get_code_status(std::string_view user_id,
                                                                        std::vector<std::string> codes) {
        std::string options {"?user_id=" + std::string(user_id)};
        for (std::string code : codes) {
            options += "&code=" + code;
        }
        std::string url {TWITCH_API_BASE + "entitlements/codes" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchCodeStatus>(response);
    }

    VectorResponse<TwitchCodeStatus> TwitchExtendedAPI::redeem_code(std::string_view user_id,
                                                                    std::vector<std::string> codes) {
        std::string options {"?user_id=" + std::string(user_id)};
        for (std::string code : codes) {
            options += "&code=" + code;
        }
        std::string url {TWITCH_API_BASE + "entitlements/codes" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_POST);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchCodeStatus>(response);
    }

    VectorResponse<TwitchDropsEntitlement> TwitchExtendedAPI::get_drops_entitlements(std::optional<std::string_view> id,
                                                                                     std::optional<std::string_view> user_id,
                                                                                     std::optional<std::string_view> game_id,
                                                                                     std::optional<std::string_view> fulfillment_status,
                                                                                     std::optional<size_t> first,
                                                                                     std::optional<std::string_view> after) {
        std::string options {""};
        if (id) {
            options += "&id=" + std::string(id.value());
        }
        if (user_id) {
            options += "&user_id=" + std::string(user_id.value());
        }
        if (game_id) {
            options += "&game_id=" + std::string(game_id.value());
        }
        if (fulfillment_status) {
            options += "&fulfillment_status=" + std::string(fulfillment_status.value());
        }
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        if (after) {
            options += "&after=" + std::string(after.value());
        }
        std::string url {TWITCH_API_BASE + "entitlements/drops" + (options.size() ? ("?" + options.substr(1, options.size())) : options)};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchDropsEntitlement>(response);
    }

    VectorResponse<TwitchDropsEntitlementStatus> TwitchExtendedAPI::update_drops_entitlements(std::string_view fulfillment_status,
                                                                                              std::vector<std::string> entitlement_ids) {
        std::string request_body {"{\"fulfillment_status\":\"" + std::string(fulfillment_status) + "\",\"entitlement_ids\":["};
        for (size_t i {0}; i < entitlement_ids.size(); ++i) {
            request_body += (i ? ",\"" : "\"") + entitlement_ids.at(i) + "\"";
        }
        request_body += "]}";
        std::string url {TWITCH_API_BASE + "entitlements/drops"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PATCH, request_body);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchDropsEntitlementStatus>(response);
    }

    VectorResponse<TwitchExtensionConfigurationSegment> TwitchExtendedAPI::get_extension_configuration_segment(std::string_view extension_id,
                                                                                                               std::vector<std::string> segments,
                                                                                                               std::optional<std::string_view> broadcaster_id) {
        std::string options {"?extension_id=" + std::string(extension_id)};
        for (std::string segment : segments) {
            options += "&segment=" + segment;
        }
        if (broadcaster_id) {
            options += "&broadcaster_id=" + std::string(broadcaster_id.value());
        }
        std::string url {TWITCH_API_BASE + "extensions/configurations" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchExtensionConfigurationSegment>(response);
    }

    Response<std::string> TwitchExtendedAPI::set_extension_configuration_segment(std::string_view extension_id,
                                                                                 std::string_view segment,
                                                                                 std::optional<std::string_view> broadcaster_id,
                                                                                 std::optional<std::string_view> content,
                                                                                 std::optional<std::string_view> version) {
        std::string request_body {"{\"extension_id\":\"" + std::string(extension_id)};
        request_body += "\",\"segment\":\"" + std::string(segment);
        if (broadcaster_id) {
            request_body += "\",\"broadcaster_id\":\"" + std::string(broadcaster_id.value());
        }
        if (content) {
            request_body += "\",\"content\":\"" + std::string(content.value());
        }
        if (version) {
            request_body += "\",\"version\":\"" + std::string(version.value());
        }
        request_body += "\"}";
        std::string url {TWITCH_API_BASE + "extensions/configurations"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PUT, request_body);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return response;
    }

    Response<std::string> TwitchExtendedAPI::set_extension_required_configuration(std::string_view broadcaster_id,
                                                                                  std::string_view extension_id,
                                                                                  std::string_view extension_version,
                                                                                  std::string_view configuration_version) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        std::string request_body {"{\"extension_id\":\"" + std::string(extension_id)
            + "\",\"extension_version\":\"" + std::string(extension_version)
            + "\",\"configuration_version\":\"" + std::string(configuration_version)
            + "\"}"};
        std::string url {TWITCH_API_BASE + "extensions/required_configuration" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PUT, request_body);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return response;
    }

    Response<std::string> TwitchExtendedAPI::send_extension_pubsub_message(std::string_view message,
                                                                           std::vector<std::string> targets,
                                                                           const bool& is_global_broadcast,
                                                                           std::optional<std::string_view> broadcaster_id) {
        std::string request_body {"{\"message\":\"" + std::string(message)
            + "\",\"target\":\"" + vector_to_json(targets)};
        if (broadcaster_id) {
            request_body += ",\"broadcaster_id\":\"" + std::string(broadcaster_id.value()) + "\"";
        }
        if (is_global_broadcast) {
            request_body += ",\"is_global_broadcast\":\"" + std::string(is_global_broadcast ? "true" : "false");
        }
        request_body += "}";
        std::string url {TWITCH_API_BASE + "extensions/pubsub"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_POST, request_body);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return response;
    }
}
