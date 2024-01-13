#pragma once

#include <dpp/dpp.h>

inline void EmbedInfoBuild(dpp::embed& embed, std::string avatar, std::string usrName, std::string usrID, std::string created, std::string BadgeShow, const dpp::user& tgtUser)
{
    embed = dpp::embed();

    embed.set_color(0xAA7EEE)
         .set_title("User Information")
         .set_thumbnail(avatar)
         .add_field("Username", usrName, true).add_field("User ID", usrID, true).add_field("Created", created, true)
         .add_field("Badges", BadgeShow)
         .set_footer(dpp::embed_footer().set_text(tgtUser.username).set_icon(tgtUser.get_avatar_url()))
         .set_timestamp(time(nullptr));
}


inline std::string StringFormatter(std::string &str)
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

void infouser(dpp::cluster& client, const dpp::slashcommand_t& event);
