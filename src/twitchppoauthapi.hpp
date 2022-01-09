#include "twitchppapi.hpp"

namespace TwitchPP {
    class TwitchOauthAPI : public TwitchAPI {
        private:
            std::string m_moderator_id {""};
        public:
            TwitchOauthAPI(const std::string& app_access_token, const std::string& client_id);
            TwitchOauthAPI(const std::string& app_access_token, const std::string& client_id, const std::string& moderator_id);
            VectorResponse<TwitchModeratorChatSettings> get_chat_settings(std::string_view broadcaster_id, bool chat_delay);
            VectorResponse<TwitchModeratorChatSettings> update_chat_settings(TwitchModeratorChatSettings& settings);
            VectorResponse<TwitchModeratorChatSettings> update_chat_settings(std::string_view broadcaster_id, std::string_view settings);
            VectorResponse<TwitchBasicUser> get_user_block_list(std::string_view broadcaster_id,
                                                                std::optional<size_t> first = std::nullopt,
                                                                std::optional<std::string_view> after = std::nullopt);
            VectorResponse<TwitchBlockedTerm> get_blocked_terms(std::string_view broadcaster_id,
                                                                std::optional<size_t> first = std::nullopt,
                                                                std::optional<std::string_view> after = std::nullopt);
            VectorResponse<TwitchAutoModSettings> get_automod_settings(std::string_view broadcaster_id);
            Response<std::string> unblock_user(std::string_view target_user_id);
            Response<std::string> block_user(std::string_view target_user_id,
                                             std::optional<std::string_view> source_context = std::nullopt,
                                             std::optional<std::string_view> reason = std::nullopt);
            VectorResponse<TwitchBlockedTerm> add_blocked_term(std::string_view broadcaster_id,
                                                               std::string_view text);
            Response<std::string> remove_blocked_term(std::string_view broadcaster_id,
                                                      std::string_view term_id);
            VectorResponse<TwitchAutoModSettings> update_automod_settings(std::string_view broadcaster_id, TwitchAutoModSettings& settings, const bool& is_overall = false);
            Response<std::string> get_stream_key(std::string_view broadcaster_id);
            VectorResponse<TwitchBasicUser> get_moderators(std::string_view broadcaster_id,
                                                           std::vector<std::string> user_ids = {},
                                                           std::optional<size_t> first = std::nullopt,
                                                           std::optional<std::string> after = std::nullopt);
            VectorResponse<TwitchBannedUser> get_banned_users(std::string_view broadcaster_id,
                                                              std::vector<std::string> user_ids = {},
                                                              std::optional<size_t> first = std::nullopt);
            VectorResponse<TwitchBannedUser> get_banned_users(std::string_view broadcaster_id,
                                                              const bool& is_after,
                                                              std::optional<size_t> first = std::nullopt,
                                                              std::optional<std::string> cursor = std::nullopt);
            VectorResponse<TwitchStream> get_followed_streams(std::string_view user_id,
                                                              std::optional<size_t> first = std::nullopt,
                                                              std::optional<std::string> after = std::nullopt);
            VectorResponse<TwitchBannedEvent> get_banned_events(std::string_view broadcaster_id,
                                                                std::vector<std::string> user_ids,
                                                                std::optional<size_t> first = std::nullopt);
            VectorResponse<TwitchBannedEvent> get_banned_events(std::string_view broadcaster_id,
                                                                std::optional<size_t> first = std::nullopt,
                                                                std::optional<std::string> cursor = std::nullopt);
            VectorResponse<TwitchModeratorEvent> get_moderator_events(std::string_view broadcaster_id,
                                                                      std::vector<std::string> user_ids,
                                                                      std::optional<size_t> first = std::nullopt);
            VectorResponse<TwitchModeratorEvent> get_moderator_events(std::string_view broadcaster_id,
                                                                      std::optional<size_t> first = std::nullopt,
                                                                      std::optional<std::string> cursor = std::nullopt);
            VectorResponse<TwitchPoll> get_polls(std::string_view broadcaster_id,
                                                 std::vector<std::string> ids = {},
                                                 std::optional<size_t> first = std::nullopt,
                                                 std::optional<std::string> after = std::nullopt);
            VectorResponse<TwitchPrediction> get_predictions(std::string_view broadcaster_id,
                                                             std::vector<std::string> ids = {},
                                                             std::optional<size_t> first = std::nullopt,
                                                             std::optional<std::string> after = std::nullopt);
            VectorResponse<TwitchCreatorsGoal> get_creators_goals(std::string_view broadcaster_id);
            VectorResponse<TwitchChannelEditor> get_channel_editors(std::string_view broadcaster_id);
            VectorResponse<TwitchUserExtension> get_user_extensions();
            VectorResponse<TwitchBitsLeaderboard> get_bits_leaderboard(size_t count = 10,
                                                                       std::optional<std::string> period = std::nullopt,
                                                                       std::optional<std::string> started_at = std::nullopt,
                                                                       std::optional<std::string> user_id = std::nullopt);
            VectorResponse<TwitchBanResponse> ban_user(std::string_view broadcaster_id,
                                                       std::string_view user_id,
                                                       std::string_view reason,
                                                       std::optional<size_t> duration = std::nullopt);
            Response<std::string> unban_user(std::string_view broadcaster_id,
                                             std::string_view user_id);
            VectorResponse<TwitchCommercialResponse> start_commercial(std::string_view broadcaster_id,
                                                                      size_t length);
            VectorResponse<TwitchAnalyticsResponse> get_extension_analytics(std::optional<AnalyticsRequest> request = std::nullopt);
            VectorResponse<TwitchAnalyticsResponse> get_game_analytics(std::optional<AnalyticsRequest> request = std::nullopt);
            Response<std::string> modify_channel_information(std::string_view broadcaster_id, ChannelInformation& info);
            VectorResponse<TwitchPoll> create_poll(std::string_view broadcaster_id,
                                                   std::string_view title,
                                                   std::vector<std::string_view> choices,
                                                   const size_t& duration,
                                                   const size_t& bits = 0,
                                                   const size_t& channel_points = 0);
            VectorResponse<TwitchPoll> end_poll(std::string_view broadcaster_id,
                                                std::string_view id,
                                                std::string_view status);
            VectorResponse<TwitchPrediction> create_prediction(std::string_view broadcaster_id,
                                                               std::string_view title,
                                                               std::vector<std::string_view> outcomes,
                                                               const size_t& prediction_window);
            VectorResponse<TwitchPrediction> end_prediction(std::string_view broadcaster_id,
                                                            std::string_view id,
                                                            std::string_view status);
            Response<std::string> update_channel_stream_schedule(std::string_view broadcaster_id,
                                                                 std::optional<bool> is_vacation_enabled = std::nullopt,
                                                                 std::optional<std::string_view> vacation_start_time = std::nullopt,
                                                                 std::optional<std::string_view> vacation_end_time = std::nullopt,
                                                                 std::optional<std::string_view> timezone = std::nullopt);
            VectorResponse<TwitchChannelStreamSchedule> create_channel_stream_schedule_segment(std::string_view broadcaster_id,
                                                                                               std::string_view start_time,
                                                                                               std::string_view timezone,
                                                                                               const bool& is_recurring,
                                                                                               std::optional<std::string_view> duration = std::nullopt,
                                                                                               std::optional<std::string_view> title = std::nullopt,
                                                                                               std::optional<std::string_view> category_id = std::nullopt);
            Response<std::string> delete_channel_stream_schedule_segment(std::string_view broadcaster_id,
                                                                         std::string_view id);
            VectorResponse<TwitchChannelStreamSchedule> update_channel_stream_schedule_segment(std::string_view broadcaster_id,
                                                                                               std::string_view id,
                                                                                               std::optional<std::string_view> start_time = std::nullopt,
                                                                                               std::optional<std::string_view> timezone = std::nullopt,
                                                                                               std::optional<bool> is_canceled = std::nullopt,
                                                                                               std::optional<std::string_view> duration = std::nullopt,
                                                                                               std::optional<std::string_view> title = std::nullopt,
                                                                                               std::optional<std::string_view> category_id = std::nullopt);
            VectorResponse<TwitchUser> update_user(std::string_view description);
            VectorResponse<std::string> delete_videos(std::vector<std::string> ids);
            VectorResponse<TwitchCreatedClip> create_clip(std::string_view broadcaster_id,
                                                          std::optional<bool> has_delay = std::nullopt);
            VectorResponse<TwitchBroadcasterSubscriptions> get_broadcaster_subscriptions(std::string_view broadcaster_id,
                                                                                         std::vector<std::string> user_ids = {},
                                                                                         std::optional<size_t> first = std::nullopt,
                                                                                         std::optional<std::string> after = std::nullopt);
            VectorResponse<TwitchCustomReward> get_custom_reward(std::string_view broadcaster_id,
                                                                 std::vector<std::string> ids = {},
                                                                 std::optional<bool> only_manageable_rewards = std::nullopt);
            VectorResponse<TwitchCustomReward> create_custom_rewards(std::string_view broadcaster_id,
                                                                     CreateRewardRequest& request);
            VectorResponse<TwitchCustomReward> update_custom_reward(std::string_view broadcaster_id,
                                                                    std::string_view reward_id,
                                                                    UpdateRewardRequest& request);
            Response<std::string> delete_custom_reward(std::string_view broadcaster_id,
                                                       std::string_view id);
            VectorResponse<TwitchCustomRewardRedemption> get_custom_reward_redemption(std::string_view broadcaster_id,
                                                                                      std::string_view reward_id,
                                                                                      std::vector<std::string> ids = {},
                                                                                      std::optional<std::string> status = std::nullopt,
                                                                                      std::optional<std::string> sort = std::nullopt,
                                                                                      std::optional<size_t> first = std::nullopt,
                                                                                      std::optional<std::string> after = std::nullopt);
            VectorResponse<TwitchCustomRewardRedemption> update_redemption_status(std::string_view broadcaster_id,
                                                                                  std::string_view reward_id,
                                                                                  std::string_view redemption_id,
                                                                                  std::string_view status);
            VectorResponse<TwitchUserSubscription> check_user_subscription(std::string_view broadcaster_id,
                                                                           std::string_view user_id);
            VectorResponse<TwitchStreamMarker> create_stream_marker(std::string_view user_id,
                                                                    std::optional<std::string_view> description = std::nullopt);
            // TODO: replace everywhere is_after => is_before to make things more logical
            VectorResponse<TwitchVideosWithMarkers> get_stream_markers(std::string_view user_id,
                                                                       std::string_view video_id,
                                                                       std::optional<size_t> first = std::nullopt,
                                                                       std::optional<std::string> cursor = std::nullopt,
                                                                       const bool& is_before = false);
            VectorResponse<TwitchStreamTag> get_all_stream_tags(std::vector<std::string> tag_ids = {},
                                                                std::optional<size_t> first = std::nullopt,
                                                                std::optional<std::string> after = std::nullopt);
            VectorResponse<TwitchStreamTag> get_stream_tags(std::string_view broadcaster_id);
            Response<std::string> replace_stream_tags(std::string_view broadcaster_id,
                                                      std::vector<std::string> tag_ids = {});
    };
}