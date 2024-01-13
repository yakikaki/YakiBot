#pragma once

#include <dpp/dpp.h>

inline void EmbedInfoBuild(dpp::embed& embed, const dpp::user& tgtUser)
{
    embed = dpp::embed();
    const auto invUrl = fmt::format("https://discord.com/api/oauth2/authorize?client_id=831629627507736606&permissions=8&scope=bot%20applications.commands");
    embed.set_color(0xAA7EEE)
        .set_title("YakiBot Invite Link")
        .add_field("Invite Yaki bot to your server!", fmt::format("Click the button below to [invite]({}) the bot to your server.", invUrl), true)
        .set_footer(dpp::embed_footer().set_text(tgtUser.username).set_icon(tgtUser.get_avatar_url()))
        .set_timestamp(time(nullptr));
}

void invite(dpp::cluster& client, const dpp::slashcommand_t& event);
