#include "twitchppapi.hpp"

namespace TwitchPP {
    class TwitchExtendedAPI : public TwitchAPI {
        public:
            TwitchExtendedAPI(const std::string& app_access_token,
                              const std::string& client_id);
            // TODO: Mention in documentation is_type parameter and similar to it
            VectorResponse<TwitchEventSubSubscriptions> get_eventsub_subscriptions(std::optional<std::string_view> filter = std::nullopt,
                                                                                   bool is_type = false,
                                                                                   std::optional<std::string> after = std::nullopt);
            // TODO: Parameters might be optimized
            VectorResponse<TwitchEventSubSubscriptions> create_eventsub_subscription(std::string_view type,
                                                                                     std::string_view version,
                                                                                     TwitchCondition condition,
                                                                                     TwitchTransport transport);
            VectorResponse<TwitchEventSubSubscriptions> create_eventsub_subscription(std::string_view type,
                                                                                     std::string_view version,
                                                                                     std::string_view condition,
                                                                                     std::string_view transport);
    };
}
