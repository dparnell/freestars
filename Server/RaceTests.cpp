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

auto fs = cmrc::test_data::get_filesystem();

DummyMessageSink messageSink;
FreeStars::Game game;

void setup() {
	TiXmlDocument rulesDoc;
	auto rules_xml = fs.open("test_data/MyModRules.xml");
	rulesDoc.Parse((const char*)rules_xml.cbegin());
	auto def = rulesDoc.FirstChild("RulesDefinition");

	FreeStars::Rules::LoadRules(def->FirstChild("Rules"), "test_data", "", 1.0, messageSink);
	FreeStars::RacialTrait::LoadRacialTraits(&game, def->FirstChild("RacialTraits"));
	game.LoadComponents(def->FirstChild("Components"));
}

void testImmuneJoatAdvantagePoints()
{
	ALEPH_TEST_BEGIN("test advantage points calculation for JOAT");

	TiXmlDocument raceDoc;
	auto race_xml = fs.open("test_data/immune-joat.r0");
	raceDoc.Parse((const char*)race_xml.cbegin());
	FreeStars::Race r;
	r.ParseNode(raceDoc.FirstChildElement("RaceDefinition"), false, messageSink);

	long points = r.GetAdvantagePoints(messageSink);
	ALEPH_ASSERT_EQUAL(points, -3900); // value as calculated by Stars! 2.70

	ALEPH_TEST_END();
}

void testJoatAdvantagePoints()
{
	ALEPH_TEST_BEGIN("test advantage points calculation for JOAT");

	TiXmlDocument raceDoc;
	auto race_xml = fs.open("test_data/default.r0");
	raceDoc.Parse((const char*)race_xml.cbegin());
	FreeStars::Race r;
	r.ParseNode(raceDoc.FirstChildElement("RaceDefinition"), false, messageSink);

	long points = r.GetAdvantagePoints(messageSink);
	ALEPH_ASSERT_EQUAL(points, 25); // value as calculated by Stars! 2.70

	ALEPH_TEST_END();
}

int main(int, char**)
{
	setup();

	testImmuneJoatAdvantagePoints();
	testJoatAdvantagePoints();
}