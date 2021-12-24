#include "twitchpputils.hpp"

namespace TwitchPP {
    /* Twitch API */
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
    constexpr std::string_view USER_MANAGE_BLOCKED_USERS { "user:manage:blocked_users" };
    constexpr std::string_view USER_READ_BLOCKED_USERS { "user:read:blocked_users" };
    constexpr std::string_view USER_READ_BROADCAST { "user:read:broadcast" };
    constexpr std::string_view USER_READ_EMAIL { "user:read:email" };
    constexpr std::string_view USER_READ_FOLLOWS { "user:read:follows" };
    constexpr std::string_view USER_READ_SUBSCRIPTIONS { "user:read:subscriptions" };

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

    /* HTTP request types */
    constexpr std::string_view HTTP_GET { "GET" };
    constexpr std::string_view HTTP_POST { "POST" };
    constexpr std::string_view HTTP_PATCH { "PATCH" };

    template<typename T> using VectorResponse = TwitchPP::Response<std::vector<T>>;

    struct StreamsRequest {
        std::optional<size_t> first = std::nullopt;
        std::optional<std::string> cursor = std::nullopt;
        bool is_after = true;
        std::vector<std::string> game_ids;
        std::vector<std::string> languages;
        std::vector<std::string> user_ids;
        std::vector<std::string> user_logins;
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
        std::string user_id;
        std::string game_id;
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

    class TwitchBasicUser {
        private:
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
                         const bool& is_mature);
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
}
