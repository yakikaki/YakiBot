#pragma once

#include <functional>
#include <dpp/dpp.h>

using  commandFunc = std::function<void(dpp::cluster&, const dpp::slashcommand_t&)>;
struct commandDef;

// Build the embed template
inline void EmbedBuild(dpp::embed& embed, uint32_t col, std::string title, std::string fieldTitle, std::string fieldDes, const dpp::user& tgtUser)
{
	embed = dpp::embed()
	.set_color(col)
	.set_title(title)
	.add_field(fieldTitle, fieldDes)
	.set_footer(dpp::embed_footer().set_text(tgtUser.username).set_icon(tgtUser.get_avatar_url()))
	.set_timestamp(time(nullptr));
}

// Build command definition and function
struct commandDef
{
	std::string                      description;
	commandFunc                      function;
	std::vector<dpp::command_option> param = {};
};
