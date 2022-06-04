#include "tests.h"
#include "FSServer.h"

#include "Game.h"
#include "Galaxy.h"

#include <cmrc/cmrc.hpp>

CMRC_DECLARE(test_data);

class DummyMessageSink : public FreeStars::MessageSink {
	FreeStars::Message* AddMessage(const std::string& type)
	{
		std::auto_ptr<FreeStars::Message> mess(new FreeStars::Message(type));
		return mess.release();
	}
};

void testJoatAdvantagePoints()
{
	ALEPH_TEST_BEGIN("test advantage points calculation for JOAT");

	auto fs = cmrc::test_data::get_filesystem();

	DummyMessageSink messageSink;

	TiXmlDocument rulesDoc;
	auto rules_xml = fs.open("test_data/MyModRules.xml");
	rulesDoc.Parse((const char*)rules_xml.cbegin());
	auto def = rulesDoc.FirstChild("RulesDefinition");

	FreeStars::Game game;
	FreeStars::Rules::LoadRules(def->FirstChild("Rules"), "test_data", "", 1.0, messageSink);
	FreeStars::RacialTrait::LoadRacialTraits(&game, def->FirstChild("RacialTraits"));
	game.LoadComponents(def->FirstChild("Components"));

	TiXmlDocument raceDoc;
	auto race_xml = fs.open("test_data/default.r0");
	raceDoc.Parse((const char*)race_xml.cbegin());
	FreeStars::Race r;
	r.ParseNode(raceDoc.FirstChildElement("RaceDefinition"), false, messageSink);

	long points = r.GetAdvantagePoints(messageSink);
	ALEPH_ASSERT_EQUAL(points, 25);

	ALEPH_TEST_END();
}

int main(int, char**)
{
	testJoatAdvantagePoints();
}