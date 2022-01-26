#include "twitchppapi.hpp"

namespace TwitchPP {
    class TwitchExtendedAPI : public TwitchAPI {
        public:
            TwitchExtendedAPI(const std::string& app_access_token,
                              const std::string& client_id);

            VectorResponse<TwitchEventSubSubscriptions> get_eventsub_subscriptions(std::optional<std::string_view> filter = std::nullopt,
                                                                                   const bool& is_type = false,
                                                                                   std::optional<std::string> after = std::nullopt);

            VectorResponse<TwitchEventSubSubscriptions> create_eventsub_subscription(std::string_view type,
                                                                                     std::string_view version,
                                                                                     std::string_view condition,
                                                                                     std::string_view transport);

            Response<std::string> delete_eventsub_subscription(std::string_view subscription_id);

            VectorResponse<TwitchCodeStatus> get_code_status(std::string_view user_id,
                                                             const std::vector<std::string>& codes);

            VectorResponse<TwitchCodeStatus> redeem_code(std::string_view user_id,
                                                         const std::vector<std::string>& codes);

            VectorResponse<TwitchDropsEntitlement> get_drops_entitlements(std::optional<std::string_view> id = std::nullopt,
                                                                          std::optional<std::string_view> user_id = std::nullopt,
                                                                          std::optional<std::string_view> game_id = std::nullopt,
                                                                          std::optional<std::string_view> fulfillment_status = std::nullopt,
                                                                          std::optional<size_t> first = std::nullopt,
                                                                          std::optional<std::string_view> after = std::nullopt);

            VectorResponse<TwitchDropsEntitlementStatus> update_drops_entitlements(std::string_view fulfillment_status,
                                                                                   const std::vector<std::string>& entitlement_ids);

            VectorResponse<TwitchExtensionConfigurationSegment> get_extension_configuration_segment(std::string_view extension_id,
                                                                                                    const std::vector<std::string>& segments,
                                                                                                    std::optional<std::string_view> broadcaster_id = std::nullopt);

            Response<std::string> set_extension_configuration_segment(std::string_view extension_id,
                                                                      std::string_view segment,
                                                                      std::optional<std::string_view> broadcaster_id = std::nullopt,
                                                                      std::optional<std::string_view> content = std::nullopt,
                                                                      std::optional<std::string_view> version = std::nullopt);

            Response<std::string> set_extension_required_configuration(std::string_view broadcaster_id,
                                                                       std::string_view extension_id,
                                                                       std::string_view extension_version,
                                                                       std::string_view configuration_version);

            Response<std::string> send_extension_pubsub_message(std::string_view message,
                                                                const std::vector<std::string>& targets,
                                                                const bool& is_global_broadcast = false,
                                                                std::optional<std::string_view> broadcaster_id = std::nullopt);

            Response<std::string> send_extension_chat_message(std::string_view broadcaster_id,
                                                              std::string_view text,
                                                              std::string_view extension_id,
                                                              std::string_view extension_version);

            VectorResponse<TwitchExtensionSecrets> get_extension_secrets(std::string_view extension_id);

            VectorResponse<TwitchExtensionSecrets> create_extension_secret(std::string_view extension_id,
                                                                           std::optional<size_t> delay = std::nullopt);

            VectorResponse<TwitchExtensionLiveChannel> get_extension_live_channels(std::string_view extension_id,
                                                                                   std::optional<size_t> first = std::nullopt,
                                                                                   std::optional<std::string_view> after = std::nullopt);

            VectorResponse<TwitchExtensionBitsProduct> get_extension_bits_products(const bool& is_global_broadcast = false);

            VectorResponse<TwitchExtensionBitsProduct> update_extension_bits_product(const std::string& sku,
                                                                                     const std::string& display_name,
                                                                                     const std::string& cost_type,
                                                                                     const size_t& cost_amount,
                                                                                     std::optional<std::string> expiration = std::nullopt,
                                                                                     const bool& in_development = false,
                                                                                     const bool& is_broadcast = false);
            VectorResponse<TwitchExtensionTransaction> get_extension_transactions(std::string_view extension_id,
                                                                                  const std::vector<std::string>& ids = {},
                                                                                  std::optional<size_t> first = std::nullopt,
                                                                                  std::optional<std::string> after = std::nullopt);

            VectorResponse<TwitchExtension> get_extensions(std::string_view extension_id,
                                                           std::optional<std::string_view> extension_version = std::nullopt);

            VectorResponse<TwitchExtension> get_released_extensions(std::string_view extension_id,
                                                                    std::optional<std::string_view> extension_version = std::nullopt);
    };
}
