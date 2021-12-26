#include "twitchpptypes.hpp"

TwitchPP::invalid_oauth::invalid_oauth(const std::string& message)
    : m_message {message} {
}

std::string_view TwitchPP::invalid_oauth::what() const { return this->m_message; }

TwitchPP::missing_permission::missing_permission(const std::string& permission)
    : m_permission {permission} {
}

std::string_view TwitchPP::missing_permission::what() const { return "OAuth token has no permission: " + this->m_permission; }

TwitchPP::TwitchEmote::TwitchEmote(const std::string& json, const std::string& emote_template) {
    this->m_id = TwitchPP::get_object_param("\"id\"", json);
    this->m_name = TwitchPP::get_object_param("\"name\"", json);
    this->m_url_1x = TwitchPP::get_object_param("\"url_1x\"", json);
    this->m_url_2x = TwitchPP::get_object_param("\"url_2x\"", json);
    this->m_url_4x = TwitchPP::get_object_param("\"url_4x\"", json);
    this->m_format = TwitchPP::json_to_vector(TwitchPP::get_object_param("\"format\"", json));
    this->m_scale = TwitchPP::json_to_vector(TwitchPP::get_object_param("\"scale\"", json));
    this->m_theme_mode = TwitchPP::json_to_vector(TwitchPP::get_object_param("\"theme_mode\"", json));
    this->m_template = emote_template;
}

TwitchPP::TwitchEmote::TwitchEmote(const std::string& id,
                                   const std::string& name,
                                   const std::string& url_1x,
                                   const std::string& url_2x,
                                   const std::string& url_4x,
                                   std::vector<std::string>& format,
                                   std::vector<std::string>& scale,
                                   std::vector<std::string>& theme_mode,
                                   const std::string& emote_template)
                                   : m_id{id},
                                     m_name{name},
                                     m_url_1x{url_1x},
                                     m_url_2x{url_2x},
                                     m_url_4x{url_4x},
                                     m_format{format},
                                     m_scale{scale},
                                     m_theme_mode{theme_mode},
                                     m_template{emote_template} {
}

std::string TwitchPP::TwitchEmote::to_json() {
    std::string json = "{\"id\":\"" + this->m_id
        + "\",\"name\":\"" + this->m_name
        + "\",\"images\":{"
        + "\"url_1x\":\"" + this->m_url_1x
        + "\",\"url_2x\":\"" + this->m_url_2x
        + "\",\"url_4x\":\"" + this->m_url_4x
        + "\"},"
        + "\"format\":" + TwitchPP::vector_to_json(this->m_format)
        + ",\"scale\":" + TwitchPP::vector_to_json(this->m_scale)
        + ",\"theme_mode\":" + TwitchPP::vector_to_json(this->m_theme_mode)
        + ",\"template\":\"" + this->m_template + "\"}";
    return json;
}

TwitchPP::TwitchChannelEmote::TwitchChannelEmote(const std::string& id,
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
                                                 std::optional<std::string_view> owner_id)
                                                 : TwitchEmote{id, name, url_1x, url_2x, url_4x, format, scale, theme_mode, emote_template},
                                                   m_emote_type{emote_type},
                                                   m_emote_set_id{emote_set_id},
                                                   m_tier{tier},
                                                   m_owner_id{owner_id.value_or("")} {
}

TwitchPP::TwitchChannelEmote::TwitchChannelEmote(const std::string& json, const std::string& emote_template, std::optional<std::string_view> owner_id)
                                                : TwitchPP::TwitchEmote{json, emote_template} {
    this->m_emote_type = TwitchPP::get_object_param("\"emote_type\"", json);
    this->m_emote_set_id = TwitchPP::get_object_param("\"emote_set_id\"", json);
    this->m_tier = TwitchPP::get_object_param("\"tier\"", json);
    this->m_owner_id = owner_id.value_or(TwitchPP::get_object_param("\"owner_id\"", json));
}

std::string TwitchPP::TwitchChannelEmote::to_json() {
    std::string json = "{\"id\":\"" + this->m_id
        + "\",\"name\":\"" + this->m_name
        + "\",\"emote_type\":\"" + this->m_emote_type
        + "\",\"emote_set_id\":\"" + this->m_emote_set_id
        + "\",\"tier\":\"" + this->m_tier
        + "\",\"owner_id\":\"" + this->m_owner_id
        + "\",\"images\":{"
        + "\"url_1x\":\"" + this->m_url_1x
        + "\",\"url_2x\":\"" + this->m_url_2x
        + "\",\"url_4x\":\"" + this->m_url_4x
        + "\"},"
        + "\"format\":" + TwitchPP::vector_to_json(this->m_format)
        + ",\"scale\":" + TwitchPP::vector_to_json(this->m_scale)
        + ",\"theme_mode\":" + TwitchPP::vector_to_json(this->m_theme_mode)
        + ",\"template\":\"" + this->m_template + "\"}";
    return json;
}

TwitchPP::TwitchBasicUser::TwitchBasicUser(const std::string& json) {
    this->m_user_id = get_object_param("\"user_id\"", json);
    this->m_user_name = get_object_param("\"user_name\"", json);
    this->m_user_login = get_object_param("\"user_login\"", json);
    if (this->m_user_name == "") {
        this->m_user_name = get_object_param("\"display_name\"", json);
    }
}

TwitchPP::TwitchBasicUser::TwitchBasicUser(const std::string& user_id, const std::string& user_name, const std::string& user_login)
    : m_user_id{user_id}, m_user_name{user_name}, m_user_login{user_login} {
}

std::string TwitchPP::TwitchBasicUser::to_json() {
    std::string json = "{\"user_id\":\"" + this->m_user_id
        + "\",\"user_name\":\"" + this->m_user_name
        + "\",\"user_login\":\"" + this->m_user_login
        + "\"}";
    return json;
}

TwitchPP::TwitchBannedUser::TwitchBannedUser(const std::string& json) : TwitchPP::TwitchBasicUser{json} {
    this->m_moderator_id = get_object_param("\"moderator_id\"", json);
    this->m_moderator_login = get_object_param("\"moderator_login\"", json);
    this->m_moderator_name = get_object_param("\"moderator_name\"", json);
    this->m_expires_at = get_object_param("\"expires_at\"", json);
    this->m_reason = get_object_param("\"reason\"", json);
}

TwitchPP::TwitchBannedUser::TwitchBannedUser(const std::string& user_id,
                                             const std::string& user_name,
                                             const std::string& user_login,
                                             const std::string& moderator_id,
                                             const std::string& moderator_login,
                                             const std::string& moderator_name,
                                             const std::string& expires_at,
                                             const std::string& reason)
                                             : TwitchPP::TwitchBasicUser{user_id, user_name, user_login},
                                               m_moderator_id{moderator_id},
                                               m_moderator_login{moderator_login},
                                               m_moderator_name{moderator_name},
                                               m_expires_at{expires_at},
                                               m_reason{reason} {
}

std::string TwitchPP::TwitchBannedUser::to_json() {
    std::string json = "{\"user_id\":\"" + this->m_user_id
        + "\",\"user_name\":\"" + this->m_user_name
        + "\",\"user_login\":\"" + this->m_user_login
        + "\",\"moderator_id\":\"" + this->m_moderator_id
        + "\",\"moderator_login\":\"" + this->m_moderator_login
        + "\",\"moderator_name\":\"" + this->m_moderator_name
        + "\",\"expires_at\":\"" + this->m_expires_at
        + "\",\"reason\":\"" + this->m_reason
        + "\"}";
    return json;
}

TwitchPP::TwitchBannedUserEx::TwitchBannedUserEx(const std::string& json) : TwitchPP::TwitchBannedUser{json} {
    this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
    this->m_broadcaster_login = get_object_param("\"broadcaster_login\"", json);
    this->m_broadcaster_name = get_object_param("\"broadcaster_name\"", json);
}

TwitchPP::TwitchBannedUserEx::TwitchBannedUserEx(const std::string& user_id,
                                                 const std::string& user_name,
                                                 const std::string& user_login,
                                                 const std::string& moderator_id,
                                                 const std::string& moderator_login,
                                                 const std::string& moderator_name,
                                                 const std::string& expires_at,
                                                 const std::string& reason,
                                                 const std::string& broadcaster_id,
                                                 const std::string& broadcaster_login,
                                                 const std::string& broadcaster_name)
                                                 : TwitchPP::TwitchBannedUser{user_id,
                                                                              user_name,
                                                                              user_login,
                                                                              moderator_id,
                                                                              moderator_login,
                                                                              moderator_name,
                                                                              expires_at,
                                                                              reason},
                                                   m_broadcaster_id{broadcaster_id},
                                                   m_broadcaster_login{broadcaster_login},
                                                   m_broadcaster_name{broadcaster_name} {
}

std::string TwitchPP::TwitchBannedUserEx::to_json() {
    std::string json = "{\"user_id\":\"" + this->m_user_id
        + "\",\"user_name\":\"" + this->m_user_name
        + "\",\"user_login\":\"" + this->m_user_login
        + "\",\"moderator_id\":\"" + this->m_moderator_id
        + "\",\"moderator_login\":\"" + this->m_moderator_login
        + "\",\"moderator_name\":\"" + this->m_moderator_name
        + "\",\"expires_at\":\"" + this->m_expires_at
        + "\",\"reason\":\"" + this->m_reason
        + "\",\"broadcaster_id\":\"" + this->m_broadcaster_id
        + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
        + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
        + "\"}";
    return json;
}

TwitchPP::TwitchBannedEvent::TwitchBannedEvent(const std::string& json) {
    this->m_id = get_object_param("\"id\"", json);
    this->m_event_type = get_object_param("\"event_type\"", json);
    this->m_event_timestamp = get_object_param("\"event_timestamp\"", json);
    this->m_version = get_object_param("\"version\"", json);
    this->m_event_data = std::make_shared<TwitchBannedUserEx>(get_object_param("\"event_data\"", json));
}

TwitchPP::TwitchBannedEvent::TwitchBannedEvent(const std::string& id,
                                               const std::string& event_type,
                                               const std::string& event_timestamp,
                                               const std::string& version,
                                               TwitchBannedUserEx& event_data)
                                               : m_id{id},
                                                 m_event_type{event_type},
                                                 m_event_timestamp{event_timestamp},
                                                 m_version{version},
                                                 m_event_data{&event_data} {
}

std::string TwitchPP::TwitchBannedEvent::to_json() {
    std::string json = "{\"id\":\"" + this->m_id
        + "\",\"event_type\":\"" + this->m_event_type
        + "\",\"event_timestamp\":\"" + this->m_event_timestamp
        + "\",\"version\":\"" + this->m_version
        + "\",\"event_data\":" + this->m_event_data->to_json()
        + "}";
    return json;
}

TwitchPP::DatetimePeriod::DatetimePeriod(const std::string& json) {
    this->m_start_time = get_object_param("\"start_time\"", json);
    this->m_end_time = get_object_param("\"end_time\"", json);
}

TwitchPP::DatetimePeriod::DatetimePeriod(const std::string& start_time, const std::string& end_time)
    : m_start_time{start_time}, m_end_time{end_time} {
}

std::string TwitchPP::DatetimePeriod::to_json() {
    std::string json = "{\"start_time\":\"" + this->m_start_time
        + "\",\"end_time\":\"" + this->m_end_time
        + "\"}";
    return json;
}

TwitchPP::TwitchCategory::TwitchCategory(const std::string& json) {
    this->m_id = TwitchPP::get_object_param("\"id\"", json);
    this->m_name = TwitchPP::get_object_param("\"name\"", json);
}

TwitchPP::TwitchCategory::TwitchCategory(const std::string& id, const std::string& name)
    : m_id{id}, m_name{name} {
}

std::string TwitchPP::TwitchCategory::get_id() {
    return this->m_id;
}

std::string TwitchPP::TwitchCategory::get_name() {
    return this->m_name;
}

std::string TwitchPP::TwitchCategory::to_json() {
    return "{\"id\":\"" + this->m_id + "\",\"name\":\"" + this->m_name + "\"}";
}

TwitchPP::TwitchGame::TwitchGame(const std::string& json) : TwitchPP::TwitchCategory{json} {
    this->m_box_art_url = TwitchPP::get_object_param("\"box_art_url\"", json);
}

TwitchPP::TwitchGame::TwitchGame(const std::string& id, const std::string& name, const std::string& box_art_url)
    : TwitchPP::TwitchCategory{id, name}, m_box_art_url{box_art_url} {
}

std::string TwitchPP::TwitchGame::get_box_art_url(const size_t& width, const size_t& height) {
    std::string sized_url {this->m_box_art_url};
    size_t width_start = sized_url.find("{width}");
    sized_url.replace(width_start, 7, std::to_string(width));
    size_t height_start = sized_url.find("{height}", width_start);
    sized_url.replace(height_start, 8, std::to_string(height));
    return sized_url;
}

std::string TwitchPP::TwitchGame::to_json() {
    std::string category_json = TwitchPP::TwitchCategory::to_json();
    return category_json.substr(0, category_json.size() - 1) + ",\"box_art_url\":\"" + this->m_box_art_url + "\"}";
}

TwitchPP::TwitchUser::TwitchUser(const std::string& json) {
    this->m_id = TwitchPP::get_object_param("\"id\"", json);
    this->m_login = TwitchPP::get_object_param("\"login\"", json);
    this->m_display_name = TwitchPP::get_object_param("\"display_name\"", json);
    this->m_type = TwitchPP::get_object_param("\"type\"", json);
    this->m_broadcaster_type = TwitchPP::get_object_param("\"broadcaster_type\"", json);
    this->m_description = TwitchPP::get_object_param("\"description\"", json);
    this->m_profile_image_url = TwitchPP::get_object_param("\"profile_image_url\"", json);
    this->m_offline_image_url = TwitchPP::get_object_param("\"offline_image_url\"", json);
    this->m_created_at = TwitchPP::get_object_param("\"created_at\"", json);
    this->m_view_count = std::stoul(TwitchPP::get_object_param("\"view_count\"", json));
    this->m_email = TwitchPP::get_object_param("\"email\"", json);
}

TwitchPP::TwitchUser::TwitchUser(const std::string& id,
                                 const std::string& login,
                                 const std::string& display_name,
                                 const std::string& type,
                                 const std::string& broadcaster_type,
                                 const std::string& description,
                                 const std::string& profile_image_url,
                                 const std::string& offline_image_url,
                                 const std::string& created_at,
                                 const size_t& view_count,
                                 const std::string& email)
                                 : m_id{id},
                                   m_login{login},
                                   m_display_name{display_name},
                                   m_type{type},
                                   m_broadcaster_type{broadcaster_type},
                                   m_description{description},
                                   m_profile_image_url{profile_image_url},
                                   m_offline_image_url{offline_image_url},
                                   m_created_at{created_at},
                                   m_view_count{view_count},
                                   m_email{email} {
}

std::string TwitchPP::TwitchUser::get_id() {
    return this->m_id;
}

std::string TwitchPP::TwitchUser::to_json() {
    std::string json = "{\"id\":\"" + this->m_id
        + "\",\"login\":\"" + this->m_login
        + "\",\"display_name\":\"" + this->m_display_name
        + "\",\"type\":\"" + this->m_type
        + "\",\"broadcaster_type\":\"" + this->m_broadcaster_type
        + "\",\"description\":\"" + this->m_description
        + "\",\"profile_image_url\":\"" + this->m_profile_image_url
        + "\",\"offline_image_url\":\"" + this->m_offline_image_url
        + "\",\"created_at\":\"" + this->m_created_at
        + "\",\"view_count\":" + std::to_string(this->m_view_count);
    if (this->m_email != "") {
        json += ",\"email\":\"" + this->m_email + "\"";
    }
    json += "}";
    return json;
}

TwitchPP::TwitchChannelInformation::TwitchChannelInformation(const std::string& json) {
    this->m_broadcaster_id = TwitchPP::get_object_param("\"broadcaster_id\"", json);
    this->m_broadcaster_login = TwitchPP::get_object_param("\"broadcaster_login\"", json);
    this->m_broadcaster_name = TwitchPP::get_object_param("\"broadcaster_name\"", json);
    this->m_game_name = TwitchPP::get_object_param("\"game_name\"", json);
    this->m_game_id = TwitchPP::get_object_param("\"game_id\"", json);
    this->m_broadcaster_language = TwitchPP::get_object_param("\"broadcaster_language\"", json);
    this->m_title = TwitchPP::get_object_param("\"title\"", json);
    this->m_delay = std::stoul(TwitchPP::get_object_param("\"delay\"", json));
}

TwitchPP::TwitchChannelInformation::TwitchChannelInformation(const std::string& broadcaster_id,
                                                             const std::string& broadcaster_login,
                                                             const std::string& broadcaster_name,
                                                             const std::string& game_name,
                                                             const std::string& game_id,
                                                             const std::string& broadcaster_language,
                                                             const std::string& title,
                                                             const size_t& delay)
                                                             : m_broadcaster_id{broadcaster_id},
                                                               m_broadcaster_login{broadcaster_login},
                                                               m_broadcaster_name{broadcaster_name},
                                                               m_game_name{game_name},
                                                               m_game_id{game_id},
                                                               m_broadcaster_language{broadcaster_language},
                                                               m_title{title},
                                                               m_delay{delay} {
}

std::string TwitchPP::TwitchChannelInformation::to_json() {
    std::string json = "{\"broadcaster_id\":\"" + this->m_broadcaster_id
        + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
        + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
        + "\",\"broadcaster_language\":\"" + this->m_broadcaster_language
        + "\",\"game_id\":\"" + this->m_game_id
        + "\",\"game_name\":\"" + this->m_game_name
        + "\",\"title\":\"" + this->m_title
        + "\",\"delay\":" + std::to_string(this->m_delay)
        + "}";
    return json;
}

TwitchPP::TwitchChatSettings::TwitchChatSettings(const std::string& json) {
    std::string smwt {TwitchPP::get_object_param("\"slow_mode_wait_time\"", json)};
    std::string fmd {TwitchPP::get_object_param("\"follower_mode_duration\"", json)};
    this->m_broadcaster_id = TwitchPP::get_object_param("\"broadcaster_id\"", json);
    this->m_slow_mode = TwitchPP::get_object_param("\"slow_mode\"", json) == "true";
    this->m_slow_mode_wait_time = smwt == "null" || smwt == "" ? -1 : std::stoi(smwt);
    this->m_follower_mode = TwitchPP::get_object_param("\"follower_mode\"", json) == "true";
    this->m_follower_mode_duration = fmd == "null" || fmd == "" ? -1 : std::stoi(fmd);
    this->m_subscriber_mode = TwitchPP::get_object_param("\"subscriber_mode\"", json) == "true";
    this->m_emote_mode = TwitchPP::get_object_param("\"emote_mode\"", json) == "true";
    this->m_unique_chat_mode = TwitchPP::get_object_param("\"unique_chat_mode\"", json) == "true";
}

TwitchPP::TwitchChatSettings::TwitchChatSettings(const std::string& broadcaster_id,
                                                 bool slow_mode,
                                                 int slow_mode_wait_time,
                                                 bool follower_mode,
                                                 int follower_mode_duration,
                                                 bool subscriber_mode,
                                                 bool emote_mode,
                                                 bool unique_chat_mode)
                                                 : m_broadcaster_id{broadcaster_id},
                                                   m_slow_mode{slow_mode},
                                                   m_slow_mode_wait_time{slow_mode_wait_time},
                                                   m_follower_mode{follower_mode},
                                                   m_follower_mode_duration{follower_mode_duration},
                                                   m_subscriber_mode{subscriber_mode},
                                                   m_emote_mode{emote_mode},
                                                   m_unique_chat_mode{unique_chat_mode} {
}

std::string TwitchPP::TwitchChatSettings::to_json() {
    return "{\"broadcaster_id\":\"" + this->m_broadcaster_id
        + "\",\"slow_mode\":" + (this->m_slow_mode ? "true" : "false")
        + ",\"slow_mode_wait_time\":" + (this->m_slow_mode_wait_time > -1 ? std::to_string(this->m_slow_mode_wait_time) : "null")
        + ",\"follower_mode\":" + (this->m_follower_mode ? "true" : "false")
        + ",\"follower_mode_duration\":" + (this->m_follower_mode_duration > -1 ? std::to_string(this->m_follower_mode_duration) : "null")
        + ",\"subscriber_mode\":" + (this->m_subscriber_mode ? "true" : "false")
        + ",\"emote_mode\":" + (this->m_emote_mode ? "true" : "false")
        + ",\"unique_chat_mode\":" + (this->m_unique_chat_mode ? "true" : "false")
        + "}";
}

TwitchPP::TwitchModeratorChatSettings::TwitchModeratorChatSettings(const std::string& json)
        : TwitchChatSettings{json} {
    std::string nmcdd {TwitchPP::get_object_param("\"non_moderator_chat_delay_duration\"", json)};
    this->m_moderator_id = TwitchPP::get_object_param("\"moderator_id\"", json);
    this->m_non_moderator_chat_delay = TwitchPP::get_object_param("\"non_moderator_chat_delay\"", json) == "true";
    this->m_non_moderator_chat_delay_duration = nmcdd == "null" || nmcdd == "" ? -1 : std::stoi(nmcdd);
}

TwitchPP::TwitchModeratorChatSettings::TwitchModeratorChatSettings(const std::string& broadcaster_id,
                                                                   bool slow_mode,
                                                                   int slow_mode_wait_time,
                                                                   bool follower_mode,
                                                                   int follower_mode_duration,
                                                                   bool subscriber_mode,
                                                                   bool emote_mode,
                                                                   bool unique_chat_mode,
                                                                   const std::string& moderator_id,
                                                                   bool non_moderator_chat_delay,
                                                                   int non_moderator_chat_delay_duration)
                                                                   : TwitchChatSettings{broadcaster_id, slow_mode, slow_mode_wait_time, follower_mode, follower_mode_duration, subscriber_mode, emote_mode, unique_chat_mode},
                                                                     m_moderator_id{moderator_id},
                                                                     m_non_moderator_chat_delay{non_moderator_chat_delay},
                                                                     m_non_moderator_chat_delay_duration{non_moderator_chat_delay_duration} {
}

std::string TwitchPP::TwitchModeratorChatSettings::to_json() {
    std::string json = TwitchPP::TwitchChatSettings::to_json();
    return json.substr(0, json.size() - 1)
        + ",\"moderator_id\":\"" + this->m_broadcaster_id
        + "\",\"non_moderator_chat_delay\":" + (this->m_follower_mode ? "true" : "false")
        + ",\"non_moderator_chat_delay_duration\":" + (this->m_follower_mode_duration > -1 ? std::to_string(this->m_follower_mode_duration) : "null")
        + "}";
}

std::string TwitchPP::TwitchModeratorChatSettings::get_broadcaster_id() {
    return this->m_broadcaster_id;
}

std::string TwitchPP::TwitchModeratorChatSettings::to_request() {
    std::string json = std::string("{\"slow_mode\":") + (this->m_slow_mode ? "true" : "false")
        + ",\"slow_mode_wait_time\":" + (this->m_slow_mode_wait_time > -1 ? std::to_string(this->m_slow_mode_wait_time) : "null")
        + ",\"follower_mode\":" + (this->m_follower_mode ? "true" : "false")
        + ",\"follower_mode_duration\":" + (this->m_follower_mode_duration > -1 ? std::to_string(this->m_follower_mode_duration) : "null")
        + ",\"subscriber_mode\":" + (this->m_subscriber_mode ? "true" : "false")
        + ",\"emote_mode\":" + (this->m_emote_mode ? "true" : "false")
        + ",\"unique_chat_mode\":" + (this->m_unique_chat_mode ? "true" : "false")
        + ",\"non_moderator_chat_delay\":" + (this->m_follower_mode ? "true" : "false")
        + ",\"non_moderator_chat_delay_duration\":" + (this->m_follower_mode_duration > -1 ? std::to_string(this->m_follower_mode_duration) : "null");
    return json + "}";
}

TwitchPP::TwitchChannel::TwitchChannel(const std::string& json) {
    this->m_id = TwitchPP::get_object_param("\"id\"", json);
    this->m_broadcaster_language = TwitchPP::get_object_param("\"broadcaster_language\"", json);
    this->m_broadcaster_login = TwitchPP::get_object_param("\"broadcaster_login\"", json);
    this->m_display_name = TwitchPP::get_object_param("\"display_name\"", json);
    this->m_game_id = TwitchPP::get_object_param("\"game_id\"", json);
    this->m_game_name = TwitchPP::get_object_param("\"game_name\"", json);
    this->m_is_live = TwitchPP::get_object_param("\"is_live\"", json) == "true";
    this->m_tags_ids = TwitchPP::json_to_vector(TwitchPP::get_object_param("\"tags_ids\"", json));
    this->m_thumbnail_url = TwitchPP::get_object_param("\"thumbnail_url\"", json);
    this->m_title = TwitchPP::get_object_param("\"title\"", json);
    this->m_started_at = TwitchPP::get_object_param("\"started_at\"", json);
}

TwitchPP::TwitchChannel::TwitchChannel(const std::string& id,
                                       const std::string& broadcaster_language,
                                       const std::string& broadcaster_login,
                                       const std::string& display_name,
                                       const std::string& game_id,
                                       const std::string& game_name,
                                       const bool& is_live,
                                       std::vector<std::string>& tags_ids,
                                       const std::string& thumbnail_url,
                                       const std::string& title,
                                       const std::string& started_at)
                                       : m_id{id},
                                         m_broadcaster_language{broadcaster_language},
                                         m_broadcaster_login{broadcaster_login},
                                         m_display_name{display_name},
                                         m_game_id{game_id},
                                         m_game_name{game_name},
                                         m_is_live{is_live},
                                         m_tags_ids{tags_ids},
                                         m_thumbnail_url{thumbnail_url},
                                         m_title{title},
                                         m_started_at{started_at} {
}

std::string TwitchPP::TwitchChannel::to_json() {
    std::string json = "{\"id\":\"" + this->m_id
        + "\",\"broadcaster_language\":\"" + this->m_broadcaster_language
        + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
        + "\",\"display_name\":\"" + this->m_display_name
        + "\",\"game_id\":\"" + this->m_game_id
        + "\",\"game_name\":\"" + this->m_game_name
        + "\",\"emote_mode\":" + (this->m_is_live ? "true" : "false")
        + ",\"tags_ids\":" + TwitchPP::vector_to_json(this->m_tags_ids)
        + ",\"thumbnail_url\":" + this->m_thumbnail_url
        + "\",\"title\":\"" + this->m_title
        + "\",\"started_at\":\"" + this->m_started_at
        + "\"}";
    return json;
}

TwitchPP::TwitchChannelTeam::TwitchChannelTeam(const std::string& json) {
    this->m_id = TwitchPP::get_object_param("\"id\"", json);
    this->m_broadcaster_id = TwitchPP::get_object_param("\"broadcaster_id\"", json);
    this->m_broadcaster_name = TwitchPP::get_object_param("\"broadcaster_name\"", json);
    this->m_broadcaster_login = TwitchPP::get_object_param("\"broadcaster_login\"", json);
    this->m_background_image_url = TwitchPP::get_object_param("\"background_image_url\"", json);
    this->m_banner = TwitchPP::get_object_param("\"banner\"", json);
    this->m_created_at = TwitchPP::get_object_param("\"created_at\"", json);
    this->m_updated_at = TwitchPP::get_object_param("\"updated_at\"", json);
    this->m_info = TwitchPP::get_object_param("\"info\"", json);
    this->m_thumbnail_url = TwitchPP::get_object_param("\"thumbnail_url\"", json);
    this->m_team_name = TwitchPP::get_object_param("\"team_name\"", json);
    this->m_team_display_name = TwitchPP::get_object_param("\"team_display_name\"", json);
}

TwitchPP::TwitchChannelTeam::TwitchChannelTeam(const std::string& id,
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
                                               const std::string& team_display_name)
                                               : m_id{id},
                                                 m_broadcaster_id{broadcaster_id},
                                                 m_broadcaster_name{broadcaster_name},
                                                 m_broadcaster_login{broadcaster_login},
                                                 m_background_image_url{background_image_url},
                                                 m_banner{banner},
                                                 m_created_at{created_at},
                                                 m_updated_at{updated_at},
                                                 m_info{info},
                                                 m_thumbnail_url{thumbnail_url},
                                                 m_team_name{team_name},
                                                 m_team_display_name{team_display_name} {
}

std::string TwitchPP::TwitchChannelTeam::to_json() {
    std::string json = "{\"id\":\"" + this->m_id
        + "\",\"broadcaster_id\":\"" + this->m_broadcaster_id
        + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
        + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
        + "\",\"background_image_url\":" + (this->m_background_image_url == "null" ? "null" : ("\"" + this->m_background_image_url + "\""))
        + ",\"banner\":\"" + (this->m_banner == "null" ? "null" : ("\"" + this->m_banner + "\""))
        + ",\"created_at\":\"" + this->m_created_at
        + "\",\"updated_at\":\"" + this->m_updated_at
        + "\",\"info\":\"" + this->m_info
        + "\",\"thumbnail_url\":\"" + this->m_thumbnail_url
        + "\",\"team_name\":\"" + this->m_team_name
        + "\",\"team_display_name\":\"" + this->m_team_display_name
        + "\"}";
    return json;
}

TwitchPP::TwitchStream::TwitchStream(const std::string& json) {
    this->m_id = TwitchPP::get_object_param("\"id\"", json);
    this->m_user_id = TwitchPP::get_object_param("\"user_id\"", json);
    this->m_user_name = TwitchPP::get_object_param("\"user_name\"", json);
    this->m_user_login = TwitchPP::get_object_param("\"user_login\"", json);
    this->m_game_id = TwitchPP::get_object_param("\"game_id\"", json);
    this->m_game_name = TwitchPP::get_object_param("\"game_name\"", json);
    this->m_type = TwitchPP::get_object_param("\"type\"", json);
    this->m_title = TwitchPP::get_object_param("\"title\"", json);
    this->m_started_at = TwitchPP::get_object_param("\"started_at\"", json);
    this->m_language = TwitchPP::get_object_param("\"language\"", json);
    this->m_thumbnail_url = TwitchPP::get_object_param("\"thumbnail_url\"", json);
    this->m_tag_ids = TwitchPP::json_to_vector(TwitchPP::get_object_param("\"tag_ids\"", json));
    this->m_viewer_count = std::stoul(TwitchPP::get_object_param("\"viewer_count\"", json));
    this->m_is_mature = TwitchPP::get_object_param("\"is_mature\"", json) == "true";
}

TwitchPP::TwitchStream::TwitchStream(const std::string& id,
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
                                     const bool& is_mature)
                                     : m_id{id},
                                       m_user_id{user_id},
                                       m_user_name{user_name},
                                       m_user_login{user_login},
                                       m_game_id{game_id},
                                       m_game_name{game_name},
                                       m_type{type},
                                       m_title{title},
                                       m_started_at{started_at},
                                       m_language{language},
                                       m_thumbnail_url{thumbnail_url},
                                       m_tag_ids{tag_ids},
                                       m_viewer_count{viewer_count},
                                       m_is_mature{is_mature} {
}

std::string TwitchPP::TwitchStream::to_json() {
    std::string json = "{\"id\":\"" + this->m_id
        + "\",\"user_id\":\"" + this->m_user_id
        + "\",\"user_name\":\"" + this->m_user_name
        + "\",\"user_login\":\"" + this->m_user_login
        + "\",\"game_id\":\"" + this->m_game_id
        + "\",\"game_name\":\"" + this->m_game_name
        + "\",\"type\":\"" + this->m_type
        + "\",\"title\":\"" + this->m_title
        + "\",\"started_at\":\"" + this->m_started_at
        + "\",\"language\":\"" + this->m_language
        + "\",\"thumbnail_url\":\"" + this->m_thumbnail_url
        + "\",\"tags_ids\":" + TwitchPP::vector_to_json(this->m_tag_ids)
        + ",\"viewer_count\":" + std::to_string(this->m_viewer_count)
        + ",\"is_mature\":" + (this->m_is_mature ? "true" : "false")
        + "}";
    return json;
}

TwitchPP::TwitchUsersFollows::TwitchUsersFollows(const std::string& json) {
    this->m_from_id = TwitchPP::get_object_param("\"from_id\"", json);
    this->m_from_login = TwitchPP::get_object_param("\"from_login\"", json);
    this->m_from_name = TwitchPP::get_object_param("\"from_name\"", json);
    this->m_to_id = TwitchPP::get_object_param("\"to_id\"", json);
    this->m_to_name = TwitchPP::get_object_param("\"to_name\"", json);
    this->m_followed_at = TwitchPP::get_object_param("\"followed_at\"", json);
}

TwitchPP::TwitchUsersFollows::TwitchUsersFollows(const std::string& from_id,
                                                 const std::string& from_login,
                                                 const std::string& from_name,
                                                 const std::string& to_id,
                                                 const std::string& to_name,
                                                 const std::string& followed_at)
                                                 : m_from_id{from_id},
                                                   m_from_login{from_login},
                                                   m_from_name{from_name},
                                                   m_to_id{to_id},
                                                   m_to_name{to_name},
                                                   m_followed_at{followed_at} {
}

std::string TwitchPP::TwitchUsersFollows::to_json() {
    std::string json = "{\"from_id\":\"" + this->m_from_id
        + "\",\"from_login\":\"" + this->m_from_login
        + "\",\"from_name\":\"" + this->m_from_name
        + "\",\"to_id\":\"" + this->m_to_id
        + "\",\"to_name\":\"" + this->m_to_name
        + "\",\"followed_at\":\"" + this->m_followed_at
        + "\"}";
    return json;
}

TwitchPP::TwitchClips::TwitchClips(const std::string& json) {
    this->m_id = TwitchPP::get_object_param("\"id\"", json);
    this->m_url = TwitchPP::get_object_param("\"url\"", json);
    this->m_embed_url = TwitchPP::get_object_param("\"embed_url\"", json);
    this->m_broadcaster_id = TwitchPP::get_object_param("\"broadcaster_id\"", json);
    this->m_broadcaster_name = TwitchPP::get_object_param("\"broadcaster_name\"", json);
    this->m_creator_id = TwitchPP::get_object_param("\"creator_id\"", json);
    this->m_creator_name = TwitchPP::get_object_param("\"creator_name\"", json);
    this->m_video_id = TwitchPP::get_object_param("\"video_id\"", json);
    this->m_game_id = TwitchPP::get_object_param("\"game_id\"", json);
    this->m_language = TwitchPP::get_object_param("\"language\"", json);
    this->m_title = TwitchPP::get_object_param("\"title\"", json);
    this->m_view_count = std::stoul(TwitchPP::get_object_param("\"view_count\"", json));
    this->m_created_at = TwitchPP::get_object_param("\"created_at\"", json);
    this->m_thumbnail_url = TwitchPP::get_object_param("\"thumbnail_url\"", json);
    this->m_duration = std::stoul(TwitchPP::get_object_param("\"duration\"", json));
}

TwitchPP::TwitchClips::TwitchClips(const std::string& id,
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
                                   const size_t& duration)
                                   : m_id{id},
                                     m_url{url},
                                     m_embed_url{embed_url},
                                     m_broadcaster_id{broadcaster_id},
                                     m_broadcaster_name{broadcaster_name},
                                     m_creator_id{creator_id},
                                     m_creator_name{creator_name},
                                     m_video_id{video_id},
                                     m_game_id{game_id},
                                     m_language{language},
                                     m_title{title},
                                     m_view_count{view_count},
                                     m_created_at{created_at},
                                     m_thumbnail_url{thumbnail_url},
                                     m_duration{duration} {
}

std::string TwitchPP::TwitchClips::to_json() {
    std::string json = "{\"id\":\"" + this->m_id
        + "\",\"url\":\"" + this->m_url
        + "\",\"embed_url\":\"" + this->m_embed_url
        + "\",\"broadcaster_id\":\"" + this->m_broadcaster_id
        + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
        + "\",\"creator_id\":\"" + this->m_creator_id
        + "\",\"creator_name\":\"" + this->m_creator_name
        + "\",\"video_id\":\"" + this->m_video_id
        + "\",\"game_id\":\"" + this->m_game_id
        + "\",\"language\":\"" + this->m_language
        + "\",\"title\":\"" + this->m_title
        + "\",\"view_count\":" + std::to_string(this->m_view_count)
        + ",\"created_at\":\"" + this->m_created_at
        + "\",\"thumbnail_url\":\"" + this->m_thumbnail_url
        + "\",\"duration\":" + std::to_string(this->m_duration)
        + "}";
    return json;
}

TwitchPP::TwitchVideos::TwitchVideos(const std::string& json) {
    this->m_id = TwitchPP::get_object_param("\"id\"", json);
    this->m_stream_id = TwitchPP::get_object_param("\"stream_id\"", json);
    this->m_user_id = TwitchPP::get_object_param("\"user_id\"", json);
    this->m_user_login = TwitchPP::get_object_param("\"user_login\"", json);
    this->m_user_name = TwitchPP::get_object_param("\"user_name\"", json);
    this->m_title = TwitchPP::get_object_param("\"title\"", json);
    this->m_description = TwitchPP::get_object_param("\"description\"", json);
    this->m_created_at = TwitchPP::get_object_param("\"created_at\"", json);
    this->m_published_at = TwitchPP::get_object_param("\"published_at\"", json);
    this->m_url = TwitchPP::get_object_param("\"url\"", json);
    this->m_thumbnail_url = TwitchPP::get_object_param("\"thumbnail_url\"", json);
    this->m_viewable = TwitchPP::get_object_param("\"viewable\"", json);
    this->m_view_count = std::stoul(TwitchPP::get_object_param("\"view_count\"", json));
    this->m_language = TwitchPP::get_object_param("\"language\"", json);
    this->m_type = TwitchPP::get_object_param("\"type\"", json);
    this->m_duration = TwitchPP::get_object_param("\"duration\"", json);
    this->m_muted_segments = TwitchPP::json_to_segment_vector(TwitchPP::get_object_param("\"muted_segments\"", json));
}

TwitchPP::TwitchVideos::TwitchVideos(const std::string& id,
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
                                     std::vector<VideoSegment> muted_segments)
                                     : m_id{id},
                                       m_stream_id{stream_id},
                                       m_user_id{user_id},
                                       m_user_login{user_login},
                                       m_user_name{user_name},
                                       m_title{title},
                                       m_description{description},
                                       m_created_at{created_at},
                                       m_published_at{published_at},
                                       m_url{url},
                                       m_thumbnail_url{thumbnail_url},
                                       m_viewable{viewable},
                                       m_view_count{view_count},
                                       m_language{language},
                                       m_type{type},
                                       m_duration{duration},
                                       m_muted_segments{muted_segments} {
}

std::string TwitchPP::TwitchVideos::to_json() {
    std::string json = "{\"id\":\"" + this->m_id
        + "\",\"stream_id\":\"" + this->m_stream_id
        + "\",\"user_id\":\"" + this->m_user_id
        + "\",\"user_login\":\"" + this->m_user_login
        + "\",\"user_name\":\"" + this->m_user_name
        + "\",\"title\":\"" + this->m_title
        + "\",\"description\":\"" + this->m_description
        + "\",\"created_at\":\"" + this->m_created_at
        + "\",\"published_at\":\"" + this->m_published_at
        + "\",\"url\":\"" + this->m_url
        + "\",\"thumbnail_url\":\"" + this->m_thumbnail_url
        + "\",\"viewable\":\"" + this->m_viewable
        + "\",\"view_count\":" + std::to_string(this->m_view_count)
        + ",\"language\":\"" + this->m_language
        + "\",\"type\":\"" + this->m_type
        + "\",\"duration\":\"" + this->m_duration
        + "\",\"muted_segments\":[";
    for (size_t i {0}; i < this->m_muted_segments.size(); ++i) {
        json += "{\"duration\":" + std::to_string(this->m_muted_segments.at(i).duration) + ",\"offset\":" + std::to_string(this->m_muted_segments.at(i).offset) + "}";
        if (i + 1 < this->m_muted_segments.size()) {
            json += ',';
        }
    }
    return json + "]}";
}

TwitchPP::TwitchScheduleSegment::TwitchScheduleSegment(const std::string& json) {
    this->m_id = TwitchPP::get_object_param("\"id\"", json);
    this->m_start_time = TwitchPP::get_object_param("\"start_time\"", json);
    this->m_end_time = TwitchPP::get_object_param("\"end_time\"", json);
    this->m_title = TwitchPP::get_object_param("\"title\"", json);
    this->m_canceled_until = TwitchPP::get_object_param("\"canceled_until\"", json);
    this->m_category = std::make_shared<TwitchPP::TwitchCategory>(TwitchPP::get_object_param("\"category\"", json));
    this->m_is_recurring = TwitchPP::get_object_param("\"is_recurring\"", json) == "true";
}

TwitchPP::TwitchScheduleSegment::TwitchScheduleSegment(const std::string& id,
                                                       const std::string& start_time,
                                                       const std::string& end_time,
                                                       const std::string& title,
                                                       const std::string& canceled_until,
                                                       TwitchCategory& category,
                                                       const bool& is_recurring)
                                                       : m_id{id},
                                                         m_start_time{start_time},
                                                         m_end_time{end_time},
                                                         m_title{title},
                                                         m_canceled_until{canceled_until},
                                                         m_category{&category},
                                                         m_is_recurring{is_recurring} {
}

std::string TwitchPP::TwitchScheduleSegment::to_json() {
    std::string json = "{\"id\":\"" + this->m_id
        + "\",\"start_time\":\"" + this->m_start_time
        + "\",\"end_time\":\"" + this->m_end_time
        + "\",\"title\":\"" + this->m_title
        + "\",\"canceled_until\":\"" + this->m_canceled_until
        + "\",\"category\":" + (this->m_category == nullptr ? "null" : this->m_category->to_json())
        + "\",\"is_recurring\":" + (this->m_is_recurring ? "true" : "false")
        + "}";
    return json;
}

TwitchPP::TwitchChannelStreamSchedule::TwitchChannelStreamSchedule(const std::string& json) {
    std::string string_with_segments = TwitchPP::get_object_param("\"segments\"", json);
    std::vector<std::string> str_segments = TwitchPP::json_to_vector(string_with_segments);
    for (std::string segment : str_segments) {
        this->m_segments.push_back(TwitchPP::TwitchScheduleSegment(segment));
    }
    this->m_broadcaster_id = TwitchPP::get_object_param("\"broadcaster_id\"", json);
    this->m_broadcaster_name = TwitchPP::get_object_param("\"broadcaster_name\"", json);
    this->m_broadcaster_login = TwitchPP::get_object_param("\"broadcaster_login\"", json);
    std::string vacation = TwitchPP::get_object_param("\"vacation\"", json);
    if (vacation != "null") {
        this->m_vacation = std::make_shared<TwitchPP::DatetimePeriod>(vacation);
    }
}

TwitchPP::TwitchChannelStreamSchedule::TwitchChannelStreamSchedule(std::vector<TwitchScheduleSegment> segments,
                                                                   const std::string& broadcaster_id,
                                                                   const std::string& broadcaster_name,
                                                                   const std::string& broadcaster_login,
                                                                   DatetimePeriod* vacation)
                                                                  : m_segments{segments},
                                                                    m_broadcaster_id{broadcaster_id},
                                                                    m_broadcaster_name{broadcaster_name},
                                                                    m_broadcaster_login{broadcaster_login},
                                                                    m_vacation{vacation} {}

std::string TwitchPP::TwitchChannelStreamSchedule::to_json() {
    std::string json {"{\"segments\":["};
    for (size_t i {0}; i < this->m_segments.size(); ++i) {
        json += this->m_segments.at(i).to_json();
        if (i + 1 < this->m_segments.size()) {
            json += ',';
        }
    }
    json += "],\"broadcaster_id\":\"" + this->m_broadcaster_id
         + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
         + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
         + "\",\"vacation\":" + (this->m_vacation == nullptr ? "null" : this->m_vacation->to_json())
         + "}";
    return json;
}

TwitchPP::TwitchTeam::TwitchTeam(const std::string& json) {
    this->m_id = TwitchPP::get_object_param("\"id\"", json);
    this->m_team_name = TwitchPP::get_object_param("\"team_name\"", json);
    this->m_team_display_name = TwitchPP::get_object_param("\"team_display_name\"", json);
    this->m_info = TwitchPP::get_object_param("\"info\"", json);
    this->m_thumbnail_url = TwitchPP::get_object_param("\"thumbnail_url\"", json);
    this->m_banner = TwitchPP::get_object_param("\"banner\"", json);
    this->m_background_image_url = TwitchPP::get_object_param("\"background_image_url\"", json);
    this->m_created_at = TwitchPP::get_object_param("\"created_at\"", json);
    this->m_updated_at = TwitchPP::get_object_param("\"updated_at\"", json);
    std::string string_with_users = TwitchPP::get_object_param("\"users\"", json);
    std::vector<std::string> str_users = TwitchPP::json_to_vector(string_with_users);
    for (std::string user : str_users) {
        this->m_users.push_back(TwitchPP::TwitchBasicUser(user));
    }
}

TwitchPP::TwitchTeam::TwitchTeam(const std::string& id,
                                 const std::string& team_name,
                                 const std::string& team_display_name,
                                 const std::string& info,
                                 const std::string& thumbnail_url,
                                 const std::string& banner,
                                 const std::string& background_image_url,
                                 const std::string& created_at,
                                 const std::string& updated_at,
                                 std::vector<TwitchBasicUser> users)
                                 : m_id{id},
                                   m_team_name{team_name},
                                   m_team_display_name{team_display_name},
                                   m_info{info},
                                   m_thumbnail_url{thumbnail_url},
                                   m_banner{banner},
                                   m_background_image_url{background_image_url},
                                   m_created_at{created_at},
                                   m_updated_at{updated_at},
                                   m_users{users} {
}

std::string TwitchPP::TwitchTeam::to_json() {
    std::string json = "{\"id\":\"" + this->m_id
        + "\",\"team_name\":\"" + this->m_team_name
        + "\",\"team_display_name\":\"" + this->m_team_display_name
        + "\",\"info\":\"" + this->m_info
        + "\",\"thumbnail_url\":\"" + this->m_thumbnail_url
        + "\",\"banner\":\"" + this->m_banner
        + "\",\"background_image_url\":\"" + this->m_background_image_url
        + "\",\"created_at\":\"" + this->m_created_at
        + "\",\"updated_at\":\"" + this->m_updated_at
        + "\",\"users\":[";
    for (size_t i {0}; i < this->m_users.size(); ++i) {
        json += this->m_users.at(i).to_json();
        if (i + 1 < this->m_users.size()) {
            json += ',';
        }
    }
    return json + "]}";
}

TwitchPP::TwitchBadgeVersion::TwitchBadgeVersion(const std::string& json) {
    this->m_id = TwitchPP::get_object_param("\"id\"", json);
    this->m_image_url_1x = TwitchPP::get_object_param("\"image_url_1x\"", json);
    this->m_image_url_2x = TwitchPP::get_object_param("\"image_url_2x\"", json);
    this->m_image_url_4x = TwitchPP::get_object_param("\"image_url_4x\"", json);
}

TwitchPP::TwitchBadgeVersion::TwitchBadgeVersion(const std::string& id,
                                                 const std::string& image_url_1x,
                                                 const std::string& image_url_2x,
                                                 const std::string& image_url_4x)
                                                 : m_id{id},
                                                   m_image_url_1x{image_url_1x},
                                                   m_image_url_2x{image_url_2x},
                                                   m_image_url_4x{image_url_4x} {
}

std::string TwitchPP::TwitchBadgeVersion::to_json() {
    std::string json = "{\"id\":\"" + this->m_id
        + "\",\"image_url_1x\":\"" + this->m_image_url_1x
        + "\",\"image_url_2x\":\"" + this->m_image_url_2x
        + "\",\"image_url_4x\":\"" + this->m_image_url_4x
        + "\"}";
    return json;
}

TwitchPP::TwitchBadgeSet::TwitchBadgeSet(const std::string& json) {
    this->m_set_id = TwitchPP::get_object_param("\"set_id\"", json);
    std::string string_with_versions = TwitchPP::get_object_param("\"versions\"", json);
    std::vector<std::string> str_versions = TwitchPP::json_to_vector(string_with_versions);
    for (std::string version : str_versions) {
        this->m_versions.push_back(TwitchPP::TwitchBadgeVersion(version));
    }
}

TwitchPP::TwitchBadgeSet::TwitchBadgeSet(const std::string& set_id,
                                         std::vector<TwitchBadgeVersion> versions)
                                         : m_set_id{set_id},
                                           m_versions{versions} {
}

std::string TwitchPP::TwitchBadgeSet::to_json() {
    std::string json = "{\"set_id\":\"" + this->m_set_id
        + "\",\"versions\":[";
    for (size_t i {0}; i < this->m_versions.size(); ++i) {
        json += this->m_versions.at(i).to_json();
        if (i + 1 < this->m_versions.size()) {
            json += ',';
        }
    }
    json += "]}";
    return json;
}

TwitchPP::TwitchImageUrls::TwitchImageUrls(const std::string& json) {
    this->m_1 = TwitchPP::get_object_param("\"1\"", json);
    this->m_15 = TwitchPP::get_object_param("\"1.5\"", json);
    this->m_2 = TwitchPP::get_object_param("\"2\"", json);
    this->m_3 = TwitchPP::get_object_param("\"3\"", json);
    this->m_4 = TwitchPP::get_object_param("\"4\"", json);
}

TwitchPP::TwitchImageUrls::TwitchImageUrls(const std::string& url_1,
                                           const std::string& url_15,
                                           const std::string& url_2,
                                           const std::string& url_3,
                                           const std::string& url_4)
                                           : m_1{url_1},
                                             m_15{url_15},
                                             m_2{url_2},
                                             m_3{url_3},
                                             m_4{url_4} {
}

std::string TwitchPP::TwitchImageUrls::to_json() {
    std::string json = (this->m_1 != "" ? "{\"1\":\"" + this->m_1
        + "\",\"1.5\":\"" + this->m_15
        + "\",\"2\":\"" + this->m_2
        + "\",\"3\":\"" + this->m_3
        + "\",\"4\":\"" + this->m_4
        + "\"}" : "");
    return json;
}

TwitchPP::TwitchImageTypes::TwitchImageTypes(const std::string& json) {
    std::string animated_str = TwitchPP::get_object_param("\"animated\"", json);
    std::string static_str = TwitchPP::get_object_param("\"static\"", json);
    this->m_animated = (animated_str == "" ? nullptr : std::make_shared<TwitchImageUrls>(animated_str));
    this->m_static = (static_str == "" ? nullptr : std::make_shared<TwitchImageUrls>(static_str));
}

TwitchPP::TwitchImageTypes::TwitchImageTypes(TwitchImageUrls& animated,
                                             TwitchImageUrls& img_static)
                                             : m_animated{&animated},
                                               m_static{&img_static} {
}

std::string TwitchPP::TwitchImageTypes::to_json() {
    std::string json {"{"};
    if (this->m_animated) {
        json += "\"animated\":" + this->m_animated->to_json();
    }
    if (this->m_static) {
        if (this->m_animated) {
            json += ",";
        }
        json += "\"static\":" + this->m_static->to_json();
    }
    return json + "}";
}

TwitchPP::TwitchImageThemes::TwitchImageThemes(const std::string& json) {
    std::string dark_str = TwitchPP::get_object_param("\"dark\"", json);
    std::string light_str = TwitchPP::get_object_param("\"light\"", json);
    this->m_dark = (dark_str == "" ? nullptr : std::make_shared<TwitchImageTypes>(dark_str));
    this->m_light = (light_str == "" ? nullptr : std::make_shared<TwitchImageTypes>(light_str));
}

TwitchPP::TwitchImageThemes::TwitchImageThemes(TwitchImageTypes& dark,
                                               TwitchImageTypes& light)
                                               : m_dark{&dark},
                                                 m_light{&light} {
}

std::string TwitchPP::TwitchImageThemes::to_json() {
    std::string json {"{"};
    if (this->m_dark) {
        json += "\"dark\":" + this->m_dark->to_json();
    }
    if (this->m_light) {
        if (this->m_dark) {
            json += ",";
        }
        json += "\"light\":" + this->m_light->to_json();
    }
    return json + "}";
}

TwitchPP::TwitchCheermoteTier::TwitchCheermoteTier(const std::string& json) {
    this->m_id = TwitchPP::get_object_param("\"id\"", json);
    this->m_min_bits = std::stoul(TwitchPP::get_object_param("\"min_bits\"", json));
    this->m_color = TwitchPP::get_object_param("\"color\"", json);
    this->m_can_cheer = TwitchPP::get_object_param("\"can_cheer\"", json) == "true";
    this->m_show_in_bits_card = TwitchPP::get_object_param("\"show_in_bits_card\"", json) == "true";
    this->m_images = TwitchImageThemes(TwitchPP::get_object_param("\"images\"", json));
}

TwitchPP::TwitchCheermoteTier::TwitchCheermoteTier(const std::string& id,
                                                   const size_t& min_bits,
                                                   const std::string& color,
                                                   const bool& can_cheer,
                                                   const bool& show_in_bits_card,
                                                   TwitchImageThemes& images)
                                                   : m_id{id},
                                                     m_min_bits{min_bits},
                                                     m_color{color},
                                                     m_can_cheer{can_cheer},
                                                     m_show_in_bits_card{show_in_bits_card},
                                                     m_images{images} {
}

std::string TwitchPP::TwitchCheermoteTier::to_json() {
    std::string json = "{\"id\":\"" + this->m_id
        + "\",\"min_bits\":" + std::to_string(this->m_min_bits)
        + ",\"color\":\"" + this->m_color
        + "\",\"can_cheer\":" + (this->m_can_cheer ? "true" : "false")
        + ",\"show_in_bits_card\":" + (this->m_show_in_bits_card ? "true" : "false")
        + ",\"images\":" + this->m_images.to_json()
        + "}";
    return json;
}

TwitchPP::TwitchCheermote::TwitchCheermote(const std::string& json) {
    this->m_prefix = TwitchPP::get_object_param("\"prefix\"", json);
    this->m_type = TwitchPP::get_object_param("\"type\"", json);
    this->m_order = std::stoul(TwitchPP::get_object_param("\"order\"", json));
    this->m_last_updated = TwitchPP::get_object_param("\"last_updated\"", json);
    this->m_is_charitable = TwitchPP::get_object_param("\"is_charitable\"", json) == "true";
    std::string string_with_tiers = TwitchPP::get_object_param("\"tiers\"", json);
    std::vector<std::string> str_tiers = TwitchPP::json_to_vector(string_with_tiers);
    for (std::string tier : str_tiers) {
        this->m_tiers.push_back(TwitchPP::TwitchCheermoteTier(tier));
    }
}

TwitchPP::TwitchCheermote::TwitchCheermote(const std::string& prefix,
                                           const std::string& type,
                                           const size_t& order,
                                           const std::string& last_updated,
                                           const bool& is_charitable,
                                           std::vector<TwitchCheermoteTier> tiers)
                                           : m_prefix{prefix},
                                             m_type{type},
                                             m_order{order},
                                             m_last_updated{last_updated},
                                             m_is_charitable{is_charitable},
                                             m_tiers{tiers} {
}

std::string TwitchPP::TwitchCheermote::to_json() {
    std::string json = "{\"prefix\":\"" + this->m_prefix
        + "\",\"type\":\"" + this->m_type
        + "\",\"order\":" + std::to_string(this->m_order)
        + ",\"last_updated\":\"" + this->m_last_updated
        + "\",\"is_charitable\":" + (this->m_is_charitable ? "true" : "false")
        + ",\"tiers\":[";
    for (size_t i {0}; i < this->m_tiers.size(); ++i) {
        json += this->m_tiers.at(i).to_json();
        if (i + 1 < this->m_tiers.size()) {
            json += ',';
        }
    }
    return json + "]}";
}

TwitchPP::TwitchBlockedTerm::TwitchBlockedTerm(const std::string& json) {
    this->m_broadcaster_id = TwitchPP::get_object_param("\"broadcaster_id\"", json);
    this->m_moderator_id = TwitchPP::get_object_param("\"moderator_id\"", json);
    this->m_id = TwitchPP::get_object_param("\"id\"", json);
    this->m_text = TwitchPP::get_object_param("\"text\"", json);
    this->m_created_at = TwitchPP::get_object_param("\"created_at\"", json);
    this->m_updated_at = TwitchPP::get_object_param("\"updated_at\"", json);
    this->m_expires_at = TwitchPP::get_object_param("\"expires_at\"", json);
}

TwitchPP::TwitchBlockedTerm::TwitchBlockedTerm(const std::string& broadcaster_id,
                                               const std::string& moderator_id,
                                               const std::string& id,
                                               const std::string& text,
                                               const std::string& created_at,
                                               const std::string& updated_at,
                                               const std::string& expires_at)
                                               : m_broadcaster_id{broadcaster_id},
                                                 m_moderator_id{moderator_id},
                                                 m_id{id},
                                                 m_text{text},
                                                 m_created_at{created_at},
                                                 m_updated_at{updated_at},
                                                 m_expires_at{expires_at} {
}

std::string TwitchPP::TwitchBlockedTerm::to_json() {
    std::string json = "{\"broadcaster_id\":\"" + this->m_broadcaster_id
        + "\",\"moderator_id\":\"" + this->m_moderator_id
        + "\",\"id\":\"" + this->m_id
        + "\",\"text\":\"" + this->m_text
        + "\",\"created_at\":\"" + this->m_created_at
        + "\",\"updated_at\":\"" + this->m_updated_at
        + "\",\"expires_at\":" + (this->m_expires_at == "null" ? this->m_expires_at : ("\"" + this->m_expires_at + "\""))
        + "}";
    return json;
}

TwitchPP::TwitchAutoModSettings::TwitchAutoModSettings(const std::string& json) {
    this->m_broadcaster_id = TwitchPP::get_object_param("\"broadcaster_id\"", json);
    this->m_moderator_id = TwitchPP::get_object_param("\"moderator_id\"", json);
    std::string overall {TwitchPP::get_object_param("\"overall_level\"", json, "null")};
    this->m_overall_level = overall == "null" ? -1 : std::stoi(overall);
    this->m_disability = std::stoi(TwitchPP::get_object_param("\"disability\"", json, "0"));
    this->m_aggression = std::stoi(TwitchPP::get_object_param("\"aggression\"", json, "0"));
    this->m_sexuality_sex_or_gender = std::stoi(TwitchPP::get_object_param("\"sexuality_sex_or_gender\"", json, "0"));
    this->m_misogyny = std::stoi(TwitchPP::get_object_param("\"misogyny\"", json, "0"));
    this->m_bullying = std::stoi(TwitchPP::get_object_param("\"bullying\"", json, "0"));
    this->m_swearing = std::stoi(TwitchPP::get_object_param("\"swearing\"", json, "0"));
    this->m_race_ethnicity_or_religion = std::stoi(TwitchPP::get_object_param("\"race_ethnicity_or_religion\"", json, "0"));
    this->m_sex_based_terms = std::stoi(TwitchPP::get_object_param("\"sex_based_terms\"", json, "0"));
}

TwitchPP::TwitchAutoModSettings::TwitchAutoModSettings(const std::string& broadcaster_id,
                                                       const std::string& moderator_id,
                                                       const int& overall_level,
                                                       const int& disability,
                                                       const int& aggression,
                                                       const int& sexuality_sex_or_gender,
                                                       const int& misogyny,
                                                       const int& bullying,
                                                       const int& swearing,
                                                       const int& race_ethnicity_or_religion,
                                                       const int& sex_based_terms)
                                                       : m_broadcaster_id{broadcaster_id},
                                                         m_moderator_id{moderator_id},
                                                         m_overall_level{overall_level},
                                                         m_disability{disability},
                                                         m_aggression{aggression},
                                                         m_sexuality_sex_or_gender{sexuality_sex_or_gender},
                                                         m_misogyny{misogyny},
                                                         m_bullying{bullying},
                                                         m_swearing{swearing},
                                                         m_race_ethnicity_or_religion{race_ethnicity_or_religion},
                                                         m_sex_based_terms{sex_based_terms} {
}

std::string TwitchPP::TwitchAutoModSettings::to_json() {
    std::string json = "{\"broadcaster_id\":\"" + this->m_broadcaster_id
        + "\",\"moderator_id\":\"" + this->m_moderator_id
        + "\",\"overall_level\":" + (this->m_overall_level > -1 ? std::to_string(this->m_overall_level) : "null")
        + ",\"disability\":" + std::to_string(this->m_disability)
        + ",\"aggression\":" + std::to_string(this->m_aggression)
        + ",\"sexuality_sex_or_gender\":" + std::to_string(this->m_sexuality_sex_or_gender)
        + ",\"misogyny\":" + std::to_string(this->m_misogyny)
        + ",\"bullying\":" + std::to_string(this->m_bullying)
        + ",\"swearing\":" + std::to_string(this->m_swearing)
        + ",\"race_ethnicity_or_religion\":" + std::to_string(this->m_race_ethnicity_or_religion)
        + ",\"sex_based_terms\":" + std::to_string(this->m_sex_based_terms)
        + "}";
    return json;
}

std::string TwitchPP::TwitchAutoModSettings::to_request(const bool& is_overall) {
    std::string json = (is_overall ? ("{\"overall_level\":" + (this->m_overall_level > -1 ? std::to_string(this->m_overall_level) : "null") + "}")
        : ("{\"disability\":" + std::to_string(this->m_disability)
        + ",\"aggression\":" + std::to_string(this->m_aggression)
        + ",\"sexuality_sex_or_gender\":" + std::to_string(this->m_sexuality_sex_or_gender)
        + ",\"misogyny\":" + std::to_string(this->m_misogyny)
        + ",\"bullying\":" + std::to_string(this->m_bullying)
        + ",\"swearing\":" + std::to_string(this->m_swearing)
        + ",\"race_ethnicity_or_religion\":" + std::to_string(this->m_race_ethnicity_or_religion)
        + ",\"sex_based_terms\":" + std::to_string(this->m_sex_based_terms)
        + "}"));
    return json;
}
