#include "twitchpputils.hpp"

namespace TwitchPP {
    /* Twitch API permissions */
    constexpr std::string_view ANALYTICS_READ_EXTENSIONS { "analytics:read:extensions" };
    constexpr std::string_view ANALYTICS_READ_GAMES { "analytics:read:games" };
    constexpr std::string_view BITS_READ { "bits:read" };
    constexpr std::string_view CHANNEL_EDIT_COMMERCIAL { "channel:edit:commercial" };
    constexpr std::string_view CHANNEL_MANAGE_BROADCAST { "channel:manage:broadcast" };
    constexpr std::string_view CHANNEL_MANAGE_EXTENSIONS { "channel:manage:extensions" };
    constexpr std::string_view CHANNEL_MANAGE_POLLS { "channel:manage:polls" };
    constexpr std::string_view CHANNEL_MANAGE_PREDICTIONS { "channel:manage:predictions" };
    constexpr std::string_view CHANNEL_MANAGE_REDEMPTIONS { "channel:manage:redemptions" };
    constexpr std::string_view CHANNEL_MANAGE_SCHEDULE { "channel:manage:schedule" };
    constexpr std::string_view CHANNEL_MANAGE_VIDEOS { "channel:manage:videos" };
    constexpr std::string_view CHANNEL_READ_EDITORS { "channel:read:editors" };
    constexpr std::string_view CHANNEL_READ_GOALS { "channel:read:goals" };
    constexpr std::string_view CHANNEL_READ_HYPE_TRAIN { "channel:read:hype_train" };
    constexpr std::string_view CHANNEL_READ_POLLS { "channel:read:polls" };
    constexpr std::string_view CHANNEL_READ_PREDICTIONS { "channel:read:predictions" };
    constexpr std::string_view CHANNEL_READ_REDEMPTIONS { "channel:read:redemptions" };
    constexpr std::string_view CHANNEL_READ_STREAM_KEY { "channel:read:stream_key" };
    constexpr std::string_view CHANNEL_READ_SUBSCRIPTIONS { "channel:read:subscriptions" };
    constexpr std::string_view CLIPS_EDIT { "clips:edit" };
    constexpr std::string_view MODERATION_READ { "moderation:read" };
    constexpr std::string_view MODERATOR_MANAGE_AUTOMOD { "moderator:manage:automod" };
    constexpr std::string_view MODERATOR_MANAGE_CHAT_SETTINGS { "moderator:manage:chat_settings" };
    constexpr std::string_view MODERATOR_READ_CHAT_SETTINGS { "moderator:read:chat_settings" };
    constexpr std::string_view MODERATOR_READ_AUTOMOD_SETTINGS { "moderator:read:automod_settings" };
    constexpr std::string_view MODERATOR_MANAGE_AUTOMOD_SETTINGS { "moderator:manage:automod_settings" };
    constexpr std::string_view MODERATOR_READ_BLOCKED_TERMS { "moderator:read:blocked_terms" };
    constexpr std::string_view MODERATOR_MANAGE_BLOCKED_TERMS { "moderator:manage:blocked_terms" };
    constexpr std::string_view USER_EDIT { "user:edit" };
    constexpr std::string_view USER_EDIT_FOLLOWS { "user:edit:follows" };
    constexpr std::string_view USER_EDIT_BROADCAST { "user:edit:broadcast" };
    constexpr std::string_view USER_MANAGE_BLOCKED_USERS { "user:manage:blocked_users" };
    constexpr std::string_view USER_READ_BLOCKED_USERS { "user:read:blocked_users" };
    constexpr std::string_view USER_READ_BROADCAST { "user:read:broadcast" };
    constexpr std::string_view USER_READ_EMAIL { "user:read:email" };
    constexpr std::string_view USER_READ_FOLLOWS { "user:read:follows" };
    constexpr std::string_view USER_READ_SUBSCRIPTIONS { "user:read:subscriptions" };
    constexpr std::string_view MODERATOR_MANAGE_BANNED_USERS { "moderator:manage:banned_users" };

    /* Chat and PubSub */
    constexpr std::string_view CHANNEL_MODERATE { "channel:moderate" };
    constexpr std::string_view CHAT_EDIT { "chat:edit" };
    constexpr std::string_view CHAT_READ { "chat:read" };
    constexpr std::string_view WHISPERS_READ { "whispers:read" };
    constexpr std::string_view WHISPERS_EDIT { "whispers:edit" };

    /* Broadcaster types */
    const std::string TWITCH_AFFILIATE { "affiliate" };
    const std::string TWITCH_PARTNER { "partner" };

    /* User types */
    const std::string TWITCH_ADMIN { "admin" };
    const std::string TWITCH_GLOBAL_MOD { "global_mod" };
    const std::string TWITCH_STAFF { "staff" };

    /* Redemption statuses */
    const std::string REDEMPTION_UNFULFILLED { "UNFULFILLED" };
    const std::string REDEMPTION_FULFILLED { "FULFILLED" };
    const std::string REDEMPTION_CANCELED { "CANCELED" };

    /* HTTP request types */
    constexpr std::string_view HTTP_GET { "GET" };
    constexpr std::string_view HTTP_POST { "POST" };
    constexpr std::string_view HTTP_PATCH { "PATCH" };
    constexpr std::string_view HTTP_DELETE { "DELETE" };
    constexpr std::string_view HTTP_PUT { "PUT" };

    /* Condition ID types */
    constexpr std::string_view CONDITION_BROADCASTER { "BROADCASTER" };
    constexpr std::string_view CONDITION_USER { "USER" };
    constexpr std::string_view CONDITION_CLIENT { "CLIENT" };
    constexpr std::string_view CONDITION_EXTENSION_CLIENT { "EXTENSION_CLIENT" };

    template<typename T> using VectorResponse = TwitchPP::Response<std::vector<T>>;
    template<typename T> using VectorResponseLeftovers = TwitchPP::ResponseLeftovers<std::vector<T>>;

    struct StreamsRequest {
        std::optional<size_t> first = std::nullopt;
        std::optional<std::string> cursor = std::nullopt;
        bool is_after = true;
        std::vector<std::string> game_ids;
        std::vector<std::string> languages;
        std::vector<std::string> user_ids;
        std::vector<std::string> user_logins;
    };

    struct AnalyticsRequest {
        std::optional<std::string> id = std::nullopt;
        std::optional<size_t> first = std::nullopt;
        std::optional<std::string> started_at = std::nullopt;
        std::optional<std::string> ended_at = std::nullopt;
        std::optional<std::string> type = std::nullopt;
        std::optional<std::string> after = std::nullopt;
    };

    struct CreateRewardRequest {
        std::string title;
        size_t cost;
        std::optional<std::string> prompt = std::nullopt;
        std::optional<std::string> background_color = std::nullopt;
        std::optional<bool> is_enabled = std::nullopt;
        std::optional<bool> is_user_input_required = std::nullopt;
        std::optional<bool> should_redemptions_skip_request_queue = std::nullopt;
        std::optional<bool> is_max_per_stream_enabled = std::nullopt;
        std::optional<size_t> max_per_stream = std::nullopt;
        std::optional<bool> is_max_per_user_per_stream_enabled = std::nullopt;
        std::optional<size_t> max_per_user_per_stream = std::nullopt;
        std::optional<bool> is_global_cooldown_enabled = std::nullopt;
        std::optional<size_t> global_cooldown_seconds = std::nullopt;
    };

    struct UpdateRewardRequest {
        std::optional<std::string> title = std::nullopt;
        std::optional<size_t> cost = std::nullopt;
        std::optional<std::string> prompt = std::nullopt;
        std::optional<std::string> background_color = std::nullopt;
        std::optional<bool> is_enabled = std::nullopt;
        std::optional<bool> is_user_input_required = std::nullopt;
        std::optional<bool> is_paused = std::nullopt;
        std::optional<bool> should_redemptions_skip_request_queue = std::nullopt;
        std::optional<bool> is_max_per_stream_enabled = std::nullopt;
        std::optional<size_t> max_per_stream = std::nullopt;
        std::optional<bool> is_max_per_user_per_stream_enabled = std::nullopt;
        std::optional<size_t> max_per_user_per_stream = std::nullopt;
        std::optional<bool> is_global_cooldown_enabled = std::nullopt;
        std::optional<size_t> global_cooldown_seconds = std::nullopt;
    };

    struct ChannelInformation {
        std::optional<std::string> title = std::nullopt;
        std::optional<std::string> game_id = std::nullopt;
        std::optional<std::string> broadcaster_language = std::nullopt;
        std::optional<size_t> delay = std::nullopt;
    };

    struct ClipsRequest {
        std::string broadcaster_id;
        std::string game_id;
        std::vector<std::string> ids;
        std::optional<size_t> first = std::nullopt;
        std::optional<std::string> cursor = std::nullopt;
        bool is_after = true;
        std::optional<std::string> started_at = std::nullopt;
        std::optional<std::string> ended_at = std::nullopt;
    };

    struct VideosRequest {
        std::vector<std::string> ids;
        std::optional<std::string> user_id = std::nullopt;
        std::optional<std::string> game_id = std::nullopt;
        std::optional<size_t> first = std::nullopt;
        std::optional<std::string> cursor = std::nullopt;
        bool is_after = true;
        std::optional<std::string> language = std::nullopt;
        std::optional<std::string> period = std::nullopt;
        std::optional<std::string> sort = std::nullopt;
        std::optional<std::string> type = std::nullopt;
    };

    struct OptionalScheduleRequest {
        std::vector<std::string> ids {};
        std::optional<std::string> start_time = std::nullopt;
        std::optional<std::string> utc_offset = std::nullopt;
        std::optional<size_t> first = std::nullopt;
        std::optional<std::string> after = std::nullopt;
    };

    class invalid_oauth {
        private:
            std::string m_message {"Invalid OAuth token used"};
        public:
            explicit invalid_oauth(const std::string& message = "Invalid OAuth token used");
            std::string_view what() const;
    };

    class missing_permission {
        private:
            std::string m_permission {"no-permissions"};
        public:
            explicit missing_permission(const std::string& permission = "no-permissions");
            std::string_view what() const;
    };

    class DatetimePeriod {
        private:
            std::string m_start_time {};
            std::string m_end_time {};
        public:
            explicit DatetimePeriod(const std::string& json);
            DatetimePeriod(const std::string& start_time, const std::string& end_time);
            DatetimePeriod(const DatetimePeriod& period) : m_start_time{period.m_start_time}, m_end_time{period.m_end_time} {}
            DatetimePeriod(DatetimePeriod&& period) noexcept : m_start_time{std::move(period.m_start_time)}, m_end_time{std::move(period.m_end_time)} {}
            ~DatetimePeriod() {}
            DatetimePeriod& operator=(const DatetimePeriod& period) {
                this->m_start_time = period.m_start_time;
                this->m_end_time = period.m_end_time;
                return *this;
            }
            DatetimePeriod& operator=(const DatetimePeriod&& period) noexcept {
                this->m_start_time = std::move(period.m_start_time);
                this->m_end_time = std::move(period.m_end_time);
                return *this;
            }
            std::string to_json();
    };

    class TwitchCategory {
        protected:
            std::string m_id {""};
            std::string m_name {""};
        public:
            explicit TwitchCategory(const std::string& json);
            TwitchCategory(const std::string& id, const std::string& name);
            TwitchCategory(const TwitchCategory& category) : m_id{category.m_id}, m_name{category.m_name} {}
            TwitchCategory(TwitchCategory&& category) noexcept : m_id{std::move(category.m_id)}, m_name{std::move(category.m_name)} {}
            ~TwitchCategory() {}
            TwitchCategory& operator=(const TwitchCategory& category) {
                this->m_id = category.m_id;
                this->m_name = category.m_name;
                return *this;
            }
            TwitchCategory& operator=(const TwitchCategory&& category) noexcept {
                this->m_id = std::move(category.m_id);
                this->m_name = std::move(category.m_name);
                return *this;
            }
            std::string get_id();
            std::string get_name();
            std::string to_json();
    };

    class TwitchGame : public TwitchCategory {
        protected:
            std::string m_box_art_url {""};
        public:
            explicit TwitchGame(const std::string& json);
            TwitchGame(const std::string& id, const std::string& name, const std::string& box_art_url);
            std::string get_box_art_url(const size_t& width, const size_t& height);
            std::string to_json();
    };

    class TwitchUserExtension {
        protected:
            std::string m_id {""};
            std::string m_version {""};
            std::string m_name {""};
            bool m_can_activate {};
            std::vector<std::string> m_type {};
        public:
            explicit TwitchUserExtension(const std::string& json);
            TwitchUserExtension(const std::string& id,
                                const std::string& version,
                                const std::string& name,
                                const bool& can_activate,
                                std::vector<std::string> type);
            std::string to_json();
    };

    class TwitchChannelEditor {
        protected:
            std::string m_user_id {""};
            std::string m_user_name {""};
            std::string m_created_at {""};
        public:
            explicit TwitchChannelEditor(const std::string& json);
            TwitchChannelEditor(const std::string& user_id,
                                const std::string& user_name,
                                const std::string& created_at);
            std::string to_json();
    };

    class TwitchBasicUser {
        protected:
            std::string m_user_id {""};
            std::string m_user_name {""};
            std::string m_user_login {""};
        public:
            TwitchBasicUser(const std::string& json);
            TwitchBasicUser(const std::string& user_id,
                            const std::string& user_name,
                            const std::string& user_login);
            std::string to_json();
    };

    class TwitchAnalyticsResponse {
        protected:
            std::string m_id {""};
            std::string m_id_var {""};
            std::string m_started_at {""};
            std::string m_ended_at {""};
            std::string m_type {""};
            std::string m_url {""};
        public:
            TwitchAnalyticsResponse(const std::string& json);
            TwitchAnalyticsResponse(const std::string& id,
                                    const std::string& id_var,
                                    const std::string& started_at,
                                    const std::string& ended_at,
                                    const std::string& type,
                                    const std::string& url);
            std::string to_json();
    };

    class TwitchCommercialResponse {
        protected:
            std::string m_message {""};
            size_t m_length {};
            size_t m_retry_after {};
        public:
            TwitchCommercialResponse(const std::string& json);
            TwitchCommercialResponse(const std::string& message,
                                     const size_t& length,
                                     const size_t& retry_after);
            std::string to_json();
    };

    class TwitchBanResponse {
        protected:
            std::string m_broadcaster_id {""};
            std::string m_moderator_id {""};
            std::string m_user_id {""};
            std::string m_end_time {""};
        public:
            TwitchBanResponse(const std::string& json);
            TwitchBanResponse(const std::string& broadcaster_id,
                              const std::string& moderator_id,
                              const std::string& user_id,
                              const std::string& end_time);
            std::string to_json();
    };

    class TwitchUserBits : public TwitchBasicUser {
        protected:
            size_t m_rank {};
            size_t m_score {};
        public:
            explicit TwitchUserBits(const std::string& json);
            TwitchUserBits(const std::string& user_id,
                           const std::string& user_name,
                           const std::string& user_login,
                           const size_t& rank,
                           const size_t& score);
            std::string to_json();
    };

    class TwitchModeratorEventData : public TwitchBasicUser {
        protected:
            std::string m_broadcaster_id {""};
            std::string m_broadcaster_login {""};
            std::string m_broadcaster_name {""};
        public:
            explicit TwitchModeratorEventData(const std::string& json);
            TwitchModeratorEventData(const std::string& user_id,
                                     const std::string& user_name,
                                     const std::string& user_login,
                                     const std::string& broadcaster_id,
                                     const std::string& broadcaster_login,
                                     const std::string& broadcaster_name);
            std::string to_json();
    };

    class TwitchBannedUser : public TwitchBasicUser {
        protected:
            std::string m_moderator_id {""};
            std::string m_moderator_login {""};
            std::string m_moderator_name {""};
            std::string m_expires_at {""};
            std::string m_reason {""};
        public:
            explicit TwitchBannedUser(const std::string& json);
            TwitchBannedUser(const std::string& user_id,
                             const std::string& user_name,
                             const std::string& user_login,
                             const std::string& moderator_id,
                             const std::string& moderator_login,
                             const std::string& moderator_name,
                             const std::string& expires_at,
                             const std::string& reason);
            std::string to_json();
    };

    class TwitchBannedUserEx : public TwitchBannedUser {
        protected:
            std::string m_broadcaster_id {""};
            std::string m_broadcaster_login {""};
            std::string m_broadcaster_name {""};
        public:
            explicit TwitchBannedUserEx(const std::string& json);
            TwitchBannedUserEx(const std::string& user_id,
                               const std::string& user_name,
                               const std::string& user_login,
                               const std::string& moderator_id,
                               const std::string& moderator_login,
                               const std::string& moderator_name,
                               const std::string& expires_at,
                               const std::string& reason,
                               const std::string& broadcaster_id,
                               const std::string& broadcaster_login,
                               const std::string& broadcaster_name);
            std::string to_json();
    };

    class TwitchBannedEvent {
        protected:
            std::string m_id {""};
            std::string m_event_type {""};
            std::string m_event_timestamp {""};
            std::string m_version {""};
            std::shared_ptr<TwitchBannedUserEx> m_event_data = nullptr;
        public:
            explicit TwitchBannedEvent(const std::string& json);
            TwitchBannedEvent(const std::string& id,
                              const std::string& event_type,
                              const std::string& event_timestamp,
                              const std::string& version,
                              TwitchBannedUserEx& event_data);
            std::string to_json();
    };

    class TwitchBitsLeaderboard {
        protected:
            std::string m_started_at {""};
            std::string m_ended_at {""};
            size_t m_total {};
            std::vector<TwitchUserBits> m_users {};
        public:
            explicit TwitchBitsLeaderboard(const std::string& json, std::vector<TwitchUserBits> users);
            TwitchBitsLeaderboard(const std::string& started_at,
                                  const std::string& ended_at,
                                  const size_t& total,
                                  std::vector<TwitchUserBits> users);
            std::string to_json();
    };

    class TwitchModeratorEvent {
        protected:
            std::string m_id {""};
            std::string m_event_type {""};
            std::string m_event_timestamp {""};
            std::string m_version {""};
            std::shared_ptr<TwitchModeratorEventData> m_event_data = nullptr;
        public:
            explicit TwitchModeratorEvent(const std::string& json);
            TwitchModeratorEvent(const std::string& id,
                                 const std::string& event_type,
                                 const std::string& event_timestamp,
                                 const std::string& version,
                                 TwitchModeratorEventData& event_data);
            std::string to_json();
    };

    class TwitchUser {
        private:
            std::string m_id {""};
            std::string m_login {""};
            std::string m_display_name {""};
            std::string m_type {""};
            std::string m_broadcaster_type {""};
            std::string m_description {""};
            std::string m_profile_image_url {""};
            std::string m_offline_image_url {""};
            std::string m_created_at {""};
            size_t m_view_count {};
            std::string m_email {""};
        public:
            TwitchUser(const std::string& json);
            TwitchUser(const std::string& id,
                       const std::string& login,
                       const std::string& display_name,
                       const std::string& type,
                       const std::string& broadcaster_type,
                       const std::string& description,
                       const std::string& profile_image_url,
                       const std::string& offline_image_url,
                       const std::string& created_at,
                       const size_t& view_count,
                       const std::string& email = "");
            std::string get_id();
            std::string to_json();
    };

    class TwitchChannelInformation {
        private:
            std::string m_broadcaster_id {""};
            std::string m_broadcaster_login {""};
            std::string m_broadcaster_name {""};
            std::string m_game_name {""};
            std::string m_game_id {""};
            std::string m_broadcaster_language {""};
            std::string m_title {""};
            size_t m_delay {};
        public:
            TwitchChannelInformation(const std::string& json);
            TwitchChannelInformation(const std::string& broadcaster_id,
                                     const std::string& broadcaster_login,
                                     const std::string& broadcaster_name,
                                     const std::string& game_name,
                                     const std::string& game_id,
                                     const std::string& broadcaster_language,
                                     const std::string& title,
                                     const size_t& delay);
            std::string to_json();
    };

    class TwitchBroadcasterSubscription {
        private:
            std::string m_broadcaster_id {""};
            std::string m_broadcaster_login {""};
            std::string m_broadcaster_name {""};
            std::string m_gifter_id {""};
            std::string m_gifter_login {""};
            std::string m_gifter_name {""};
            std::string m_tier {""};
            std::string m_plan_name {""};
            std::string m_user_id {""};
            std::string m_user_name {""};
            std::string m_user_login {""};
            bool m_is_gift {};
        public:
            TwitchBroadcasterSubscription(const std::string& json);
            TwitchBroadcasterSubscription(const std::string& broadcaster_id,
                                          const std::string& broadcaster_login,
                                          const std::string& broadcaster_name,
                                          const std::string& gifter_id,
                                          const std::string& gifter_login,
                                          const std::string& gifter_name,
                                          const std::string& tier,
                                          const std::string& plan_name,
                                          const std::string& user_id,
                                          const std::string& user_name,
                                          const std::string& user_login,
                                          const bool& is_gift);
            std::string to_json();
    };

    class TwitchBroadcasterSubscriptions {
        protected:
            size_t m_total {};
            size_t m_points {};
            std::vector<TwitchBroadcasterSubscription> m_broadcaster_subscriptions {};
        public:
            TwitchBroadcasterSubscriptions(const size_t& total,
                                           const size_t& points,
                                           std::vector<TwitchBroadcasterSubscription> broadcaster_subscriptions);
            std::string to_json();
    };


    class TwitchEmote {
        protected:
            std::string m_id {""};
            std::string m_name {""};
            std::string m_url_1x {""};
            std::string m_url_2x {""};
            std::string m_url_4x {""};
            std::vector<std::string> m_format {};
            std::vector<std::string> m_scale {};
            std::vector<std::string> m_theme_mode {};
            std::string m_template {""};
        public:
            explicit TwitchEmote(const std::string& json, const std::string& emote_template);
            TwitchEmote(const std::string& id,
                        const std::string& name,
                        const std::string& url_1x,
                        const std::string& url_2x,
                        const std::string& url_4x,
                        std::vector<std::string>& format,
                        std::vector<std::string>& scale,
                        std::vector<std::string>& theme_mode,
                        const std::string& emote_template);
            std::string to_json();
    };

    class TwitchChannelEmote : public TwitchEmote {
        private:
            std::string m_emote_type {""};
            std::string m_emote_set_id {""};
            std::string m_tier {""};
            std::string m_owner_id {""};
        public:
            explicit TwitchChannelEmote(const std::string& json, const std::string& emote_template, std::optional<std::string_view> owner_id = std::nullopt);
            TwitchChannelEmote(const std::string& id,
                               const std::string& name,
                               const std::string& url_1x,
                               const std::string& url_2x,
                               const std::string& url_4x,
                               std::vector<std::string>& format,
                               std::vector<std::string>& scale,
                               std::vector<std::string>& theme_mode,
                               const std::string& emote_template,
                               const std::string& emote_type,
                               const std::string& emote_set_id,
                               const std::string& tier,
                               std::optional<std::string_view> owner_id = std::nullopt);
            std::string to_json();
    };

    class TwitchChatSettings {
        protected:
            std::string m_broadcaster_id {""};
            bool m_slow_mode {};
            int m_slow_mode_wait_time {};
            bool m_follower_mode {};
            int m_follower_mode_duration {};
            bool m_subscriber_mode {};
            bool m_emote_mode {};
            bool m_unique_chat_mode {};
        public:
            explicit TwitchChatSettings(const std::string& json);
            TwitchChatSettings(const std::string& broadcaster_id,
                               bool slow_mode,
                               int slow_mode_wait_time,
                               bool follower_mode,
                               int follower_mode_duration,
                               bool subscriber_mode,
                               bool emote_mode,
                               bool unique_chat_mode);
            std::string to_json();
    };

    class TwitchModeratorChatSettings : public TwitchChatSettings {
        private:
            std::string m_moderator_id {};
            bool m_non_moderator_chat_delay {};
            int m_non_moderator_chat_delay_duration {};
        public:
            explicit TwitchModeratorChatSettings(const std::string& json);
            TwitchModeratorChatSettings(const std::string& broadcaster_id,
                                        bool slow_mode,
                                        int slow_mode_wait_time,
                                        bool follower_mode,
                                        int follower_mode_duration,
                                        bool subscriber_mode,
                                        bool emote_mode,
                                        bool unique_chat_mode,
                                        const std::string& moderator_id,
                                        bool non_moderator_chat_delay,
                                        int non_moderator_chat_delay_duration);
            std::string get_broadcaster_id();
            std::string to_json();
            std::string to_request();
    };

    class TwitchChannel {
        protected:
            std::string m_id {""};
            std::string m_broadcaster_language {""};
            std::string m_broadcaster_login {""};
            std::string m_display_name {""};
            std::string m_game_id {""};
            std::string m_game_name {""};
            bool m_is_live {};
            std::vector<std::string> m_tags_ids {};
            std::string m_thumbnail_url {""};
            std::string m_title {""};
            std::string m_started_at {""};
        public:
            explicit TwitchChannel(const std::string& json);
            TwitchChannel(const std::string& id,
                          const std::string& broadcaster_language,
                          const std::string& broadcaster_login,
                          const std::string& display_name,
                          const std::string& game_id,
                          const std::string& game_name,
                          const bool& is_live,
                          std::vector<std::string>& tags_ids,
                          const std::string& thumbnail_url,
                          const std::string& title,
                          const std::string& started_at);
            std::string to_json();
    };

    class TwitchChannelTeam {
        protected:
            std::string m_id {""};
            std::string m_broadcaster_id {""};
            std::string m_broadcaster_name {""};
            std::string m_broadcaster_login {""};
            std::string m_background_image_url {""};
            std::string m_banner {""};
            std::string m_created_at {""};
            std::string m_updated_at {""};
            std::string m_info {""};
            std::string m_thumbnail_url {""};
            std::string m_team_name {""};
            std::string m_team_display_name {""};
        public:
            explicit TwitchChannelTeam(const std::string& json);
            TwitchChannelTeam(const std::string& id,
                              const std::string& broadcaster_id,
                              const std::string& broadcaster_name,
                              const std::string& broadcaster_login,
                              const std::string& background_image_url,
                              const std::string& banner,
                              const std::string& created_at,
                              const std::string& updated_at,
                              const std::string& info,
                              const std::string& thumbnail_url,
                              const std::string& team_name,
                              const std::string& team_display_name);
            std::string to_json();
    };

    class TwitchStream {
        protected:
            std::string m_id {""};
            std::string m_user_id {""};
            std::string m_user_name {""};
            std::string m_user_login {""};
            std::string m_game_id {""};
            std::string m_game_name {""};
            std::string m_type {""};
            std::string m_title {""};
            std::string m_started_at {""};
            std::string m_language {""};
            std::string m_thumbnail_url {""};
            std::vector<std::string> m_tag_ids {""};
            size_t m_viewer_count {};
            bool m_is_mature {};
        public:
            explicit TwitchStream(const std::string& json);
            TwitchStream(const std::string& id,
                         const std::string& user_id,
                         const std::string& user_name,
                         const std::string& user_login,
                         const std::string& game_id,
                         const std::string& game_name,
                         const std::string& type,
                         const std::string& title,
                         const std::string& started_at,
                         const std::string& language,
                         const std::string& thumbnail_url,
                         std::vector<std::string>& tag_ids,
                         const size_t& viewer_count,
                         const bool& is_mature = false);
            std::string to_json();
    };

    class TwitchUsersFollows {
        protected:
            std::string m_from_id {""};
            std::string m_from_login {""};
            std::string m_from_name {""};
            std::string m_to_id {""};
            std::string m_to_name {""};
            std::string m_followed_at {""};
        public:
            explicit TwitchUsersFollows(const std::string& json);
            TwitchUsersFollows(const std::string& from_id,
                               const std::string& from_login,
                               const std::string& from_name,
                               const std::string& to_id,
                               const std::string& to_name,
                               const std::string& followed_at);
            std::string to_json();
    };

    class TwitchClips {
        protected:
            std::string m_id {""};
            std::string m_url {""};
            std::string m_embed_url {""};
            std::string m_broadcaster_id {""};
            std::string m_broadcaster_name {""};
            std::string m_creator_id {""};
            std::string m_creator_name {""};
            std::string m_video_id {""};
            std::string m_game_id {""};
            std::string m_language {""};
            std::string m_title {""};
            size_t m_view_count {};
            std::string m_created_at {""};
            std::string m_thumbnail_url {""};
            size_t m_duration {};
        public:
            explicit TwitchClips(const std::string& json);
            TwitchClips(const std::string& id,
                        const std::string& url,
                        const std::string& embed_url,
                        const std::string& broadcaster_id,
                        const std::string& broadcaster_name,
                        const std::string& creator_id,
                        const std::string& creator_name,
                        const std::string& video_id,
                        const std::string& game_id,
                        const std::string& language,
                        const std::string& title,
                        const size_t& view_count,
                        const std::string& created_at,
                        const std::string& thumbnail_url,
                        const size_t& duration);
            std::string to_json();
    };

    class TwitchCreatedClip {
        protected:
            std::string m_id {""};
            std::string m_edit_url {""};
            size_t m_limit {};
            size_t m_remaining {};
        public:
            explicit TwitchCreatedClip(const std::string& json,
                                       const size_t& limit,
                                       const size_t& remaining);
            TwitchCreatedClip(const std::string& id,
                              const std::string& edit_url,
                              const size_t& limit,
                              const size_t& remaining);
            std::string to_json();
    };

    class TwitchVideos {
        protected:
            std::string m_id {""};
            std::string m_stream_id {""};
            std::string m_user_id {""};
            std::string m_user_login {""};
            std::string m_user_name {""};
            std::string m_title {""};
            std::string m_description {""};
            std::string m_created_at {""};
            std::string m_published_at {""};
            std::string m_url {""};
            std::string m_thumbnail_url {""};
            std::string m_viewable {""};
            size_t m_view_count {};
            std::string m_language {""};
            std::string m_type {""};
            std::string m_duration {""};
            std::vector<VideoSegment> m_muted_segments {};
        public:
            explicit TwitchVideos(const std::string& json);
            TwitchVideos(const std::string& id,
                         const std::string& stream_id,
                         const std::string& user_id,
                         const std::string& user_login,
                         const std::string& user_name,
                         const std::string& title,
                         const std::string& description,
                         const std::string& created_at,
                         const std::string& published_at,
                         const std::string& url,
                         const std::string& thumbnail_url,
                         const std::string& viewable,
                         const size_t& view_count,
                         const std::string& language,
                         const std::string& type,
                         const std::string& duration,
                         std::vector<VideoSegment> muted_segments);
            std::string to_json();
    };

    class TwitchScheduleSegment {
        protected:
            std::string m_id {""};
            std::string m_start_time {""};
            std::string m_end_time {""};
            std::string m_title {""};
            std::string m_canceled_until {""};
            std::shared_ptr<TwitchCategory> m_category = nullptr;
            bool m_is_recurring {};
        public:
            explicit TwitchScheduleSegment(const std::string& json);
            TwitchScheduleSegment(const std::string& id,
                                  const std::string& start_time,
                                  const std::string& end_time,
                                  const std::string& title,
                                  const std::string& canceled_until,
                                  TwitchCategory& category,
                                  const bool& is_recurring);
            TwitchScheduleSegment(const TwitchScheduleSegment& segment)
                : m_id{segment.m_id},
                  m_start_time{segment.m_start_time},
                  m_end_time{segment.m_end_time},
                  m_title{segment.m_title},
                  m_canceled_until{segment.m_canceled_until},
                  m_category{segment.m_category},
                  m_is_recurring{segment.m_is_recurring} {}
            TwitchScheduleSegment(TwitchScheduleSegment&& segment) noexcept
                : m_id{std::move(segment.m_id)},
                  m_start_time{std::move(segment.m_start_time)},
                  m_end_time{std::move(segment.m_end_time)},
                  m_title{std::move(segment.m_title)},
                  m_canceled_until{std::move(segment.m_canceled_until)},
                  m_category{std::move(segment.m_category)},
                  m_is_recurring{std::move(segment.m_is_recurring)} {}
            TwitchScheduleSegment& operator=(const TwitchScheduleSegment& segment) {
                this->m_id = segment.m_id;
                this->m_start_time = segment.m_start_time;
                this->m_end_time = segment.m_end_time;
                this->m_title = segment.m_title;
                this->m_canceled_until = segment.m_canceled_until;
                this->m_category = segment.m_category;
                this->m_is_recurring = segment.m_is_recurring;
                return *this;
            }
            TwitchScheduleSegment& operator=(const TwitchScheduleSegment&& segment) noexcept {
                this->m_id = std::move(segment.m_id);
                this->m_start_time = std::move(segment.m_start_time);
                this->m_end_time = std::move(segment.m_end_time);
                this->m_title = std::move(segment.m_title);
                this->m_canceled_until = std::move(segment.m_canceled_until);
                this->m_category = std::move(segment.m_category);
                this->m_is_recurring = std::move(segment.m_is_recurring);
                return *this;
            }
            ~TwitchScheduleSegment() {
                this->m_category.reset();
            }
            std::string to_json();
    };

    class TwitchChannelStreamSchedule {
        protected:
            std::vector<TwitchScheduleSegment> m_segments {};
            std::string m_broadcaster_id {""};
            std::string m_broadcaster_name {""};
            std::string m_broadcaster_login {""};
            std::shared_ptr<DatetimePeriod> m_vacation = nullptr;
        public:
            explicit TwitchChannelStreamSchedule(const std::string& json);
            TwitchChannelStreamSchedule(std::vector<TwitchScheduleSegment> segments,
                                        const std::string& broadcaster_id,
                                        const std::string& broadcaster_name,
                                        const std::string& broadcaster_login,
                                        DatetimePeriod* vacation);
            TwitchChannelStreamSchedule(const TwitchChannelStreamSchedule& schedule)
                : m_segments{schedule.m_segments},
                  m_broadcaster_id{schedule.m_broadcaster_id},
                  m_broadcaster_name{schedule.m_broadcaster_name},
                  m_broadcaster_login{schedule.m_broadcaster_login},
                  m_vacation{schedule.m_vacation} {}
            TwitchChannelStreamSchedule(TwitchChannelStreamSchedule&& schedule) noexcept
                : m_segments{std::move(schedule.m_segments)},
                  m_broadcaster_id{std::move(schedule.m_broadcaster_id)},
                  m_broadcaster_name{std::move(schedule.m_broadcaster_name)},
                  m_broadcaster_login{std::move(schedule.m_broadcaster_login)},
                  m_vacation{std::move(schedule.m_vacation)} {}
            TwitchChannelStreamSchedule& operator=(const TwitchChannelStreamSchedule& schedule) {
                this->m_segments = schedule.m_segments;
                this->m_broadcaster_id = schedule.m_broadcaster_id;
                this->m_broadcaster_name = schedule.m_broadcaster_name;
                this->m_broadcaster_login = schedule.m_broadcaster_login;
                this->m_vacation = schedule.m_vacation;
                return *this;
            }
            TwitchChannelStreamSchedule& operator=(const TwitchChannelStreamSchedule&& schedule) noexcept {
                this->m_segments = std::move(schedule.m_segments);
                this->m_broadcaster_id = std::move(schedule.m_broadcaster_id);
                this->m_broadcaster_name = std::move(schedule.m_broadcaster_name);
                this->m_broadcaster_login = std::move(schedule.m_broadcaster_login);
                this->m_vacation = std::move(schedule.m_vacation);
                return *this;
            }
            ~TwitchChannelStreamSchedule() {
                this->m_vacation.reset();
            }
            std::string to_json();
    };

    class TwitchTeam {
        protected:
            std::string m_id {""};
            std::string m_team_name {""};
            std::string m_team_display_name {""};
            std::string m_info {""};
            std::string m_thumbnail_url {""};
            std::string m_banner {""};
            std::string m_background_image_url {""};
            std::string m_created_at {""};
            std::string m_updated_at {""};
            std::vector<TwitchBasicUser> m_users {};
        public:
            explicit TwitchTeam(const std::string& json);
            TwitchTeam(const std::string& id,
                       const std::string& team_name,
                       const std::string& team_display_name,
                       const std::string& info,
                       const std::string& thumbnail_url,
                       const std::string& banner,
                       const std::string& background_image_url,
                       const std::string& created_at,
                       const std::string& updated_at,
                       std::vector<TwitchBasicUser> users);
            std::string to_json();
    };

    class TwitchBadgeVersion {
        protected:
            std::string m_id {""};
            std::string m_image_url_1x {""};
            std::string m_image_url_2x {""};
            std::string m_image_url_4x {""};
        public:
            explicit TwitchBadgeVersion(const std::string& json);
            TwitchBadgeVersion(const std::string& id,
                               const std::string& image_url_1x,
                               const std::string& image_url_2x,
                               const std::string& image_url_4x);
            std::string to_json();
    };

    class TwitchBadgeSet {
        protected:
            std::string m_set_id {""};
            std::vector<TwitchBadgeVersion> m_versions {};
        public:
            explicit TwitchBadgeSet(const std::string& json);
            TwitchBadgeSet(const std::string& set_id,
                           std::vector<TwitchBadgeVersion> versions);
            std::string to_json();
    };

    class TwitchImageUrls {
        protected:
            std::string m_1 {""};
            std::string m_15 {""};
            std::string m_2 {""};
            std::string m_3 {""};
            std::string m_4 {""};
        public:
            TwitchImageUrls() = default;
            explicit TwitchImageUrls(const std::string& json);
            TwitchImageUrls(const std::string& url_1,
                            const std::string& url_15,
                            const std::string& url_2,
                            const std::string& url_3,
                            const std::string& url_4);
            std::string to_json();
    };

    class TwitchImageTypes {
        protected:
            std::shared_ptr<TwitchImageUrls> m_animated = nullptr;
            std::shared_ptr<TwitchImageUrls> m_static = nullptr;
        public:
            TwitchImageTypes() = default;
            explicit TwitchImageTypes(const std::string& json);
            TwitchImageTypes(TwitchImageUrls& animated,
                             TwitchImageUrls& img_static);
            std::string to_json();
    };

    class TwitchImageThemes {
        protected:
            std::shared_ptr<TwitchImageTypes> m_dark = nullptr;
            std::shared_ptr<TwitchImageTypes> m_light = nullptr;
        public:
            TwitchImageThemes() = default;
            explicit TwitchImageThemes(const std::string& json);
            TwitchImageThemes(TwitchImageTypes& dark,
                              TwitchImageTypes& light);
            std::string to_json();
    };

    class TwitchCheermoteTier {
        protected:
            std::string m_id {""};
            size_t m_min_bits {};
            std::string m_color {""};
            bool m_can_cheer {};
            bool m_show_in_bits_card {};
            TwitchImageThemes m_images {};
        public:
            TwitchCheermoteTier() = default;
            explicit TwitchCheermoteTier(const std::string& json);
            TwitchCheermoteTier(const std::string& id,
                                const size_t& min_bits,
                                const std::string& color,
                                const bool& can_cheer,
                                const bool& show_in_bits_card,
                                TwitchImageThemes& images);
            std::string to_json();
    };

    class TwitchCheermote {
        protected:
            std::string m_prefix {""};
            std::string m_type {""};
            size_t m_order {};
            std::string m_last_updated {""};
            bool m_is_charitable {};
            std::vector<TwitchCheermoteTier> m_tiers {};
        public:
            explicit TwitchCheermote(const std::string& json);
            TwitchCheermote(const std::string& prefix,
                            const std::string& type,
                            const size_t& order,
                            const std::string& last_updated,
                            const bool& is_charitable,
                            std::vector<TwitchCheermoteTier> tiers);
            std::string to_json();
    };

    class TwitchBlockedTerm {
        protected:
            std::string m_broadcaster_id {""};
            std::string m_moderator_id {""};
            std::string m_id {""};
            std::string m_text {""};
            std::string m_created_at {""};
            std::string m_updated_at {""};
            std::string m_expires_at {""};
        public:
            TwitchBlockedTerm() = default;
            explicit TwitchBlockedTerm(const std::string& json);
            TwitchBlockedTerm(const std::string& broadcaster_id,
                              const std::string& moderator_id,
                              const std::string& id,
                              const std::string& text,
                              const std::string& created_at,
                              const std::string& updated_at,
                              const std::string& expires_at);
            std::string to_json();
    };

    class TwitchAutoModSettings {
        protected:
            std::string m_broadcaster_id {""};
            std::string m_moderator_id {""};
            int m_overall_level {};
            int m_disability {};
            int m_aggression {};
            int m_sexuality_sex_or_gender {};
            int m_misogyny {};
            int m_bullying {};
            int m_swearing {};
            int m_race_ethnicity_or_religion {};
            int m_sex_based_terms {};
        public:
            TwitchAutoModSettings();
            explicit TwitchAutoModSettings(const std::string& json);
            TwitchAutoModSettings(const std::string& broadcaster_id,
                                  const std::string& moderator_id,
                                  const int& overall_level,
                                  const int& disability,
                                  const int& aggression,
                                  const int& sexuality_sex_or_gender,
                                  const int& misogyny,
                                  const int& bullying,
                                  const int& swearing,
                                  const int& race_ethnicity_or_religion,
                                  const int& sex_based_terms);
            std::string to_json();
            std::string to_request(const bool& is_overall = true);
    };

    class TwitchPollChoice {
        protected:
            std::string m_id {""};
            std::string m_title {""};
            size_t m_votes {};
            size_t m_channel_points_votes {};
            size_t m_bits_votes {};
        public:
            TwitchPollChoice(const std::string& json);
            TwitchPollChoice(const std::string& id,
                             const std::string& title,
                             const size_t& votes,
                             const size_t& channel_points_votes,
                             const size_t& bits_votes);
            std::string to_json();
    };

    class TwitchPoll {
        protected:
            std::string m_id {""};
            std::string m_broadcaster_id {""};
            std::string m_broadcaster_name {""};
            std::string m_broadcaster_login{""};
            std::string m_title {""};
            std::string m_status {""};
            std::string m_started_at {""};
            bool m_bits_voting_enabled {};
            bool m_channel_points_voting_enabled {};
            size_t m_bits_per_vote {};
            size_t m_channel_points_per_vote {};
            size_t m_duration {};
            std::vector<TwitchPollChoice> m_choices {};
        public:
            TwitchPoll(const std::string& json);
            TwitchPoll(const std::string& id,
                       const std::string& broadcaster_id,
                       const std::string& broadcaster_name,
                       const std::string& broadcaster_login,
                       const std::string& title,
                       const std::string& status,
                       const std::string& started_at,
                       const bool& bits_voting_enabled,
                       const bool& channel_points_voting_enabled,
                       const size_t& bits_per_vote,
                       const size_t& channel_points_per_vote,
                       const size_t& duration,
                       std::vector<TwitchPollChoice> choices);
            std::string get_id();
            std::string to_json();
    };

    class TwitchPredictor : public TwitchBasicUser {
        protected:
            size_t m_channel_points_used {};
            size_t m_channel_points_won {};
        public:
            explicit TwitchPredictor(const std::string& json);
            TwitchPredictor(const std::string& user_id,
                            const std::string& user_name,
                            const std::string& user_login,
                            const size_t& channel_points_used,
                            const size_t& channel_points_won);
            std::string to_json();
    };

    class TwitchPredictionOutcome {
        protected:
            std::string m_id {""};
            std::string m_title {""};
            std::string m_color {""};
            size_t m_users {};
            size_t m_channel_points {};
            std::vector<TwitchPredictor> m_top_predictors {};
        public:
            TwitchPredictionOutcome(const std::string& json);
            TwitchPredictionOutcome(const std::string& id,
                                    const std::string& title,
                                    const std::string& color,
                                    const size_t& users,
                                    const size_t& channel_points,
                                    std::vector<TwitchPredictor> top_predictors);
            std::string to_json();
    };

    class TwitchPrediction {
        protected:
            std::string m_id {""};
            std::string m_broadcaster_id {""};
            std::string m_broadcaster_name {""};
            std::string m_broadcaster_login {""};
            std::string m_title {""};
            std::string m_status {""};
            std::string m_winning_outcome_id {""};
            std::string m_created_at {""};
            std::string m_ended_at {""};
            std::string m_locked_at {""};
            size_t m_prediction_window {};
            std::vector<TwitchPredictionOutcome> m_outcomes {};
        public:
            TwitchPrediction(const std::string& json);
            TwitchPrediction(const std::string& id,
                             const std::string& broadcaster_id,
                             const std::string& broadcaster_name,
                             const std::string& broadcaster_login,
                             const std::string& title,
                             const std::string& status,
                             const std::string& winning_outcome_id,
                             const std::string& created_at,
                             const std::string& ended_at,
                             const std::string& locked_at,
                             const size_t& prediction_window,
                             std::vector<TwitchPredictionOutcome> outcomes);
            std::string get_id();
            std::string to_json();
    };

    class TwitchCreatorsGoal {
        protected:
            std::string m_id {""};
            std::string m_broadcaster_id {""};
            std::string m_broadcaster_name {""};
            std::string m_broadcaster_login {""};
            std::string m_type{""};
            std::string m_description {""};
            std::string m_created_at{""};
            size_t m_current_amount {};
            size_t m_target_amount {};
        public:
            TwitchCreatorsGoal(const std::string& json);
            TwitchCreatorsGoal(const std::string& id,
                               const std::string& broadcaster_id,
                               const std::string& broadcaster_name,
                               const std::string& broadcaster_login,
                               const std::string& type,
                               const std::string& description,
                               const std::string& created_at,
                               const size_t& current_amount,
                               const size_t& target_amount);
            std::string to_json();
    };

    class TwitchCustomReward {
        public:
            std::string m_id {""};
            std::string m_broadcaster_id {""};
            std::string m_broadcaster_name {""};
            std::string m_broadcaster_login {""};
            std::string m_title {""};
            std::string m_prompt {""};
            std::string m_background_color {""};
            std::string m_cooldown_expires_at {""};
            size_t m_cost {};
            bool m_is_enabled {};
            bool m_is_user_input_required {};
            bool m_is_paused {};
            bool m_is_in_stock {};
            bool m_should_redemptions_skip_request_queue {};
            int m_redemptions_redeemed_current_stream {-1};
            bool m_max_per_stream_setting_enabled {};
            size_t m_max_per_stream {};
            bool m_max_per_user_per_stream_setting_enabled {};
            size_t m_max_per_user_per_stream {};
            bool m_global_cooldown_setting_enabled {};
            size_t m_global_cooldown_seconds {};
            std::string m_url_1x {""};
            std::string m_url_2x {""};
            std::string m_url_4x {""};
            std::string m_default_url_1x {""};
            std::string m_default_url_2x {""};
            std::string m_default_url_4x {""};

            TwitchCustomReward(const std::string& json);
            TwitchCustomReward(const std::string& id,
                               const std::string& broadcaster_id,
                               const std::string& broadcaster_name,
                               const std::string& broadcaster_login,
                               const std::string& title,
                               const std::string& prompt,
                               const std::string& background_color,
                               const std::string& cooldown_expires_at,
                               const size_t& cost,
                               const bool& is_enabled,
                               const bool& is_user_input_required,
                               const bool& is_paused,
                               const bool& is_in_stock,
                               const bool& should_redemptions_skip_request_queue,
                               const int& redemptions_redeemed_current_stream,
                               const bool& max_per_stream_setting_enabled,
                               const size_t& max_per_stream,
                               const bool& max_per_user_per_stream_setting_enabled,
                               const size_t& max_per_user_per_stream,
                               const bool& global_cooldown_setting_enabled,
                               const size_t& global_cooldown_seconds,
                               const std::string& url_1x,
                               const std::string& url_2x,
                               const std::string& url_4x,
                               const std::string& default_url_1x,
                               const std::string& default_url_2x,
                               const std::string& default_url_4x);
            std::string to_json();
    };

    class TwitchCustomRewardRedemption {
        public:
            std::string m_id {""};
            std::string m_broadcaster_id {""};
            std::string m_broadcaster_name {""};
            std::string m_broadcaster_login {""};
            std::string m_user_id {""};
            std::string m_user_name {""};
            std::string m_user_login {""};
            std::string m_user_input {""};
            std::string m_status{""};
            std::string m_redeemed_at {""};
            std::string m_reward_id {""};
            std::string m_reward_title {""};
            std::string m_reward_prompt {""};
            size_t m_reward_cost {};

            TwitchCustomRewardRedemption(const std::string& json);
            TwitchCustomRewardRedemption(const std::string& id,
                                         const std::string& broadcaster_id,
                                         const std::string& broadcaster_name,
                                         const std::string& broadcaster_login,
                                         const std::string& user_id,
                                         const std::string& user_name,
                                         const std::string& user_login,
                                         const std::string& user_input,
                                         const std::string& status,
                                         const std::string& redeemed_at,
                                         const std::string& reward_id,
                                         const std::string& reward_title,
                                         const std::string& reward_prompt,
                                         const size_t& reward_cost);
            std::string to_json();
    };

    class TwitchUserSubscription {
        protected:
            std::string m_broadcaster_id {""};
            std::string m_broadcaster_login {""};
            std::string m_broadcaster_name {""};
            std::string m_gifter_login {""};
            std::string m_gifter_name {""};
            std::string m_tier {""};
            bool m_is_gift {};
        public:
            TwitchUserSubscription(const std::string& json);
            TwitchUserSubscription(const std::string& broadcaster_id,
                                   const std::string& broadcaster_login,
                                   const std::string& broadcaster_name,
                                   const std::string& gifter_login,
                                   const std::string& gifter_name,
                                   const std::string& tier,
                                   const bool& is_gift);
            std::string to_json();
    };

    class TwitchStreamMarker {
        public:
            std::string m_id {""};
            std::string m_description {""};
            std::string m_created_at {""};
            std::string m_url {""};
            size_t m_position_seconds {};

            TwitchStreamMarker(const std::string& json);
            TwitchStreamMarker(const std::string& id,
                               const std::string& description,
                               const std::string& created_at,
                               const std::string& url,
                               const size_t& position_seconds);
            std::string to_json();
    };

    class TwitchVideoWithMarkers {
        public:
            std::string m_video_id {""};
            std::vector<TwitchStreamMarker> m_markers {};

            TwitchVideoWithMarkers(const std::string& json);
            TwitchVideoWithMarkers(const std::string& video_id,
                                   std::vector<TwitchStreamMarker> markers);
            std::string to_json();
    };

    class TwitchVideosWithMarkers {
        public:
            std::string m_user_id {""};
            std::string m_user_name {""};
            std::string m_user_login {""};
            std::vector<TwitchVideoWithMarkers> m_videos {};

            TwitchVideosWithMarkers(const std::string& json);
            TwitchVideosWithMarkers(const std::string& user_id,
                                    const std::string& user_name,
                                    const std::string& user_login,
                                    std::vector<TwitchVideoWithMarkers> videos);
            std::string to_json();
    };

    class TwitchStreamTag {
        public:
            std::string m_tag_id {""};
            bool m_is_auto {};
            StringMap m_localization_names {};
            StringMap m_localization_descriptions {};

            TwitchStreamTag(const std::string& json);
            TwitchStreamTag(const std::string& tag_id,
                            const bool& is_auto,
                            StringMap localization_names,
                            StringMap localization_descriptions);
            std::string to_json();
    };

    class TwitchAutoModMessage {
        public:
            std::string m_msg_id {""};
            std::string m_msg_text {""};
            std::string m_user_id {""};

            TwitchAutoModMessage(const std::string& json);
            TwitchAutoModMessage(const std::string& msg_id,
                                 const std::string& msg_text,
                                 const std::string& user_id);
            std::string to_json();
    };

    class TwitchAutoModMessageStatus {
        public:
            std::string m_msg_id {""};
            bool m_is_permitted {};

            TwitchAutoModMessageStatus(const std::string& json);
            TwitchAutoModMessageStatus(const std::string& msg_id,
                                       const bool& is_permitted);
            std::string to_json();
    };

    class TwitchAutoModMessageManaging {
        public:
            std::string m_msg_id {""};
            std::string m_user_id {""};
            std::string m_action {""};

            TwitchAutoModMessageManaging(const std::string& json);
            TwitchAutoModMessageManaging(const std::string& msg_id,
                                         const std::string& user_id,
                                         const std::string& action);
            std::string to_json();
    };

    class TwitchHypeTrainContribution {
        protected:
            std::string m_user {""};
            std::string m_type {""};
            size_t m_total {};
        public:
            explicit TwitchHypeTrainContribution(const std::string& json);
            TwitchHypeTrainContribution(const std::string& user,
                                        const std::string& type,
                                        const size_t& total);
            std::string to_json();
    };

    class TwitchHypeTrainData {
        protected:
            std::string m_id {""};
            std::string m_broadcaster_id {""};
            std::string m_cooldown_end_time {""};
            std::string m_started_at {""};
            std::string m_expires_at {""};
            size_t m_level {};
            size_t m_total {};
            size_t m_goal {};
            std::shared_ptr<TwitchHypeTrainContribution> m_last_contribution = nullptr;
            std::vector<TwitchHypeTrainContribution> m_top_contributions {};
        public:
            explicit TwitchHypeTrainData(const std::string& json);
            TwitchHypeTrainData(const std::string& id,
                                const std::string& broadcaster_id,
                                const std::string& cooldown_end_time,
                                const std::string& started_at,
                                const std::string& expires_at,
                                const size_t& level,
                                const size_t& total,
                                const size_t& goal,
                                std::shared_ptr<TwitchHypeTrainContribution> last_contribution = nullptr,
                                std::vector<TwitchHypeTrainContribution> top_contributions = {});
            std::string to_json();
    };

    class TwitchHypeTrainEvent {
        protected:
            std::string m_id {""};
            std::string m_event_type {""};
            std::string m_event_timestamp {""};
            std::string m_version {""};
            std::shared_ptr<TwitchHypeTrainData> m_event_data = nullptr;
        public:
            explicit TwitchHypeTrainEvent(const std::string& json);
            TwitchHypeTrainEvent(const std::string& id,
                                 const std::string& event_type,
                                 const std::string& event_timestamp,
                                 const std::string& version,
                                 TwitchHypeTrainData& event_data);
            std::string to_json();
    };

    class TwitchMusicArtist {
        protected:
            std::string m_id {""};
            std::string m_name {""};
            std::string m_creator_channel_id {""};
        public:
            TwitchMusicArtist(const std::string& json);
            TwitchMusicArtist(const std::string& id,
                              const std::string& name,
                              const std::string& creator_channel_id);
            std::string to_json();
    };

    class TwitchMusicAlbum {
        protected:
            std::string m_id {""};
            std::string m_name {""};
            std::string m_image_url {""};
        public:
            TwitchMusicAlbum(const std::string& json);
            TwitchMusicAlbum(const std::string& id,
                             const std::string& name,
                             const std::string& image_url);
            std::string to_json();
    };

    class TwitchMusicSource {
        protected:
            std::string m_id {""};
            std::string m_title {""};
            std::string m_content_type {""};
            std::string m_soundtrack_url {""};
            std::string m_spotify_url {""};
            std::string m_image_url {""};
        public:
            TwitchMusicSource(const std::string& json);
            TwitchMusicSource(const std::string& id,
                              const std::string& title,
                              const std::string& content_type,
                              const std::string& soundtrack_url,
                              const std::string& spotify_url,
                              const std::string& image_url);
            std::string to_json();
    };

    class TwitchTrack {
        protected:
            std::string m_id {""};
            std::string m_title {""};
            size_t m_duration {};
            std::shared_ptr<TwitchMusicAlbum> m_album = nullptr;
            std::vector<TwitchMusicArtist> m_artists {};
        public:
            explicit TwitchTrack(const std::string& json);
            TwitchTrack(const std::string& id,
                        const std::string& title,
                        const size_t& duration,
                        std::shared_ptr<TwitchMusicAlbum> album = nullptr,
                        std::vector<TwitchMusicArtist> artists = {});
            std::string to_json();
    };

    class TwitchCurrentTrack {
        protected:
            std::shared_ptr<TwitchTrack> m_track = nullptr;
            std::shared_ptr<TwitchMusicSource> m_source = nullptr;
        public:
            explicit TwitchCurrentTrack(const std::string& json);
            TwitchCurrentTrack(std::shared_ptr<TwitchTrack> track = nullptr,
                               std::shared_ptr<TwitchMusicSource> source = nullptr);
            std::string to_json();
    };

    class TwitchBasicPlaylist {
        protected:
            std::string m_id {""};
            std::string m_title {""};
            std::string m_description {""};
            std::string m_image_url {""};
        public:
            TwitchBasicPlaylist(const std::string& json);
            TwitchBasicPlaylist(const std::string& id,
                                const std::string& title,
                                const std::string& description,
                                const std::string& image_url);
            std::string to_json();
    };

    class TwitchPlaylist : public TwitchBasicPlaylist {
        protected:
            std::vector<TwitchTrack> m_tracks {};
        public:
            TwitchPlaylist(const std::string& json);
            TwitchPlaylist(const std::string& id,
                           const std::string& title,
                           const std::string& description,
                           const std::string& image_url,
                           std::vector<TwitchTrack> tracks = {});
            std::string to_json();
    };

    class TwitchCondition {
        protected:
            std::string m_broadcaster_user_id {""};
            std::string m_from_broadcaster_user_id {""};
            std::string m_to_broadcaster_user_id {""};
            std::string m_reward_id {""};
            std::string m_organization_id {""};
            std::string m_category_id {""};
            std::string m_campaign_id {""};
            std::string m_extension_client_id {""};
            std::string m_client_id {""};
            std::string m_user_id {""};
        public:
            TwitchCondition(const std::string& json);
            // TODO: Describe possible id_types in doc and is_reward
            TwitchCondition(const std::string& id,
                            std::string_view id_type);
            TwitchCondition(const std::string& broadcaster_user_id,
                            const std::string& seconcary_parameter,
                            const bool& is_reward);
            TwitchCondition(const std::string& organization_id,
                            const std::string& category_id,
                            const std::string& campaign_id);
            std::string to_json();
    };

    class TwitchTransport {
        protected:
            std::string m_method {""};
            std::string m_callback {""};
            std::string m_secret {""};
        public:
            TwitchTransport(const std::string& json);
            TwitchTransport(const std::string& method,
                            const std::string& callback,
                            const std::string& secret);
            std::string to_json();
    };

    class TwitchEventSubSubscription {
        protected:
            std::string m_id {""};
            std::string m_status {""};
            std::string m_type {""};
            std::string m_version {""};
            std::string m_created_at {""};
            size_t m_cost {};
            std::shared_ptr<TwitchTransport> m_transport = nullptr;
            std::shared_ptr<TwitchCondition> m_condition = nullptr;
        public:
            TwitchEventSubSubscription(const std::string& json);
            TwitchEventSubSubscription(const std::string& id,
                                       const std::string& status,
                                       const std::string& type,
                                       const std::string& version,
                                       const std::string& created_at,
                                       const size_t& cost,
                                       std::shared_ptr<TwitchTransport> transport = nullptr,
                                       std::shared_ptr<TwitchCondition> condition = nullptr);
            std::string to_json();
    };

    class TwitchEventSubSubscriptions {
        protected:
            size_t m_total {};
            size_t m_total_cost {};
            size_t m_max_total_cost {};
            std::vector<TwitchEventSubSubscription> m_subscriptions {};
        public:
            TwitchEventSubSubscriptions(const std::string& json,
                                        std::vector<TwitchEventSubSubscription> subscriptions = {});
            TwitchEventSubSubscriptions(const size_t& total,
                                        const size_t& total_cost,
                                        const size_t& max_total_cost,
                                        std::vector<TwitchEventSubSubscription> subscriptions = {});
            std::string to_json();
    };

    class TwitchUserActiveExtension {
        public:
            bool m_active {};
            std::string m_id {""};
            std::string m_name {""};
            std::string m_version {""};
            int m_x_pos {-1};
            int m_y_pos {-1};

            TwitchUserActiveExtension();
            TwitchUserActiveExtension(const std::string& json);
            TwitchUserActiveExtension(const bool& active,
                                      const std::string& id,
                                      const std::string& name,
                                      const std::string& version,
                                      const int& x_pos,
                                      const int& y_pos);
            std::string to_json();
    };

    class TwitchUserActiveExtensions {
        public:
            std::vector<TwitchUserActiveExtension> m_panel {};
            std::vector<TwitchUserActiveExtension> m_overlay {};
            std::vector<TwitchUserActiveExtension> m_component {};

            TwitchUserActiveExtensions(std::vector<TwitchUserActiveExtension> panel = {},
                                       std::vector<TwitchUserActiveExtension> overlay = {},
                                       std::vector<TwitchUserActiveExtension> component = {});
            std::string to_json();
    };

    class TwitchCodeStatus {
        protected:
            std::string m_code {""};
            std::string m_status {""};
        public:
            explicit TwitchCodeStatus(const std::string& json);
            TwitchCodeStatus(const std::string& code,
                             const std::string& status);
            std::string to_json();
    };

    class TwitchDropsEntitlement {
        protected:
            std::string m_id {""};
            std::string m_benefit_id{""};
            std::string m_timestamp {""};
            std::string m_user_id {""};
            std::string m_game_id {""};
            std::string m_fulfillment_status {""};
            std::string m_updated_at {""};
        public:
            TwitchDropsEntitlement(const std::string& json);
            TwitchDropsEntitlement(const std::string& id,
                                   const std::string& benefit_id,
                                   const std::string& timestamp,
                                   const std::string& user_id,
                                   const std::string& game_id,
                                   const std::string& fulfillment_status,
                                   const std::string& updated_at);
            std::string to_json();
    };

    class TwitchDropsEntitlementStatus {
        protected:
            std::string m_status {""};
            std::vector<std::string> m_ids {};
        public:
            explicit TwitchDropsEntitlementStatus(const std::string& json);
            TwitchDropsEntitlementStatus(const std::string& status,
                                         std::vector<std::string> ids);
            std::string to_json();
    };
}
