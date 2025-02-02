#include <spdlog/spdlog.h>
#include <dpp/dpp.h>
#include "../../commands/information/invite.h"
#include "../../handler/handler.h"


void invite(dpp::cluster& client, const dpp::slashcommand_t& event)
{
    dpp::embed embed;

    const auto errorTitle = "<:failed:1036206712916553748> Error";
    const auto warnTitle = "Warning message";

    const auto cmdUser = event.command.usr;
    auto       usrId = cmdUser.id;
  

    if (std::holds_alternative<dpp::snowflake>(event.get_parameter("user")) == true)
        usrId = std::get<dpp::snowflake>(event.get_parameter("user"));

    dpp::user_identified tgtId = client.user_get_sync(usrId);

    

    const auto avatar = tgtId.get_avatar_url();
    const auto usrID = fmt::format("{}", tgtId.id);
    const auto created = fmt::format("<t:{}:R>", round(tgtId.get_creation_time()));
    const auto usrName = fmt::format("{}", tgtId.format_username());


    // Add view profile linking button
    auto       linkComponent = dpp::component();
    const auto inviteLink = fmt::format("https://discord.com/api/oauth2/authorize?client_id=831629627507736606&permissions=8&scope=bot%20applications.commands");

    linkComponent.set_label("Invite The Bot")
        .set_type(dpp::cot_button)
        .set_emoji("MessageLink", 1045584835420368896)
        .set_style(dpp::cos_link)
        .set_url(inviteLink);

    EmbedInfoBuild(embed, cmdUser);

    event.reply(
        dpp::message().add_embed(embed).add_component(dpp::component().add_component(linkComponent)).set_flags(dpp::m_ephemeral)
    );
}