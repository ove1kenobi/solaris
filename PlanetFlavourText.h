#pragma once
#include "pch.h"

enum class planetType { warm = 0, cold, radio, normal };

class FlavourTextManager {
private:
	std::vector<std::wstring> m_warmTexts;
	std::vector<std::wstring> m_coldTexts;
	std::vector<std::wstring> m_radioTexts;
	std::vector<std::wstring> m_normalTexts;

public:
	FlavourTextManager() {
		//Warm
		m_warmTexts.push_back(
			L"On this sad excuse for a planet there's not much. Sand and more sand, with a few rocky mountaintops peeking out of the dunes."
			L" You've heard that there once was a thriving clan of nomads that roamed the deserts of this planet."
		);

		m_warmTexts.push_back(
			L"You knew it would be hot this close to the sun, but this hot? The whole planet is engulfed in a red-ish cloud of hot gas."
			L" The planet was probably green and lively once, but because of all the gasses an exponential greenhouse effect started."
		);

		m_warmTexts.push_back(
			L"This planet is filled with lava lakes, with some sort of eel-species that inhabits the fiery waters."
			L" It's hard to get around here, since you have to make your way around the endless number of small pools of lava."
		);

		m_warmTexts.push_back(
			L"A lush world, a bit too hot for humans, but it still has a lot of life."
			L" Mostly the types you see in this kind of climate, insects, camels and other animals normally seen on the savannah."
		);

		m_warmTexts.push_back(
			L"Mountainous red terrain. Dragon-like serpents slithering about in the valleys between the mountains."
			L" It seems that only the most adventurous types visit this planet."
		);

		//Cold
		m_coldTexts.push_back(
			L"The most icy-blue planet you've ever seen. The whole surface is covered in a sheet of ice that looks like an endless see from space."
		);

		m_coldTexts.push_back(
			L"Cold. So cold. The insane windspeeds on this planet makes it almost twice as cold as it would be otherwise."
			L" It's impossible to venture to certain parts of the planet because of shards of ice being scraped of the glaciers and thrown like knives through the wind."
		);

		m_coldTexts.push_back(
			L"A planet filled with icy cavern systems. The whole planet is almost like a gargantuan ant farm with all its tunnels."
			L" Who knows what could be hiding in those deeps."
		);

		m_coldTexts.push_back(
			L" The planet has a purple hue to it. There is complete silence on the surface. No wind, no plants, nothing. Atleast not where you decided to land."
		);

		m_coldTexts.push_back(
			L"This planet has two sides to it, one of them is always turned towards the sun which actually allows some plants to grow here."
			L" The other side is in complete darkness, you wouldn't even send your worst enemy there."
		);

		//Radio

		m_radioTexts.push_back(
			L"You can almost see the planet glowing, some kind of catastrophe certainly happened here."
			L" So many ruins of old capitals, cities and towns."
		);

		m_radioTexts.push_back(
			L"Filled with radioactive forests, the planet looks like it has never seen even a touch of human influence."
			L" But realistically, who knows what kind of mutant creatures could be hiding in those trees."
		);

		m_radioTexts.push_back(
			L"The humans here have adapted, some kind of immunity to the radioactive material."
			L" Sure, some of them have a few mutations here and there, but they are largely unaffected."
		);

		m_radioTexts.push_back(
			L"A purple goo covers the whole surface of this planet, even from space you can see gigantic whales and other creatures scurrying about in the waters."
			L" There are a few pieces of land where you can run about. But it's probably a wise decision to stay atleast a kilometer or two away from the green glowing beaches."
		);

		//Normal

		m_normalTexts.push_back(
			L"The plant-like organisms on this planet are made up of millions of flowers."
			L" Their combined colors and scents make for an amazing spectacle, but they leave little space for other species, which is why there are only very few bush and shrub species."
		);

		m_normalTexts.push_back(
			L"Lush and warm rainforests. Filled with all kinds of life, dinosaurs, gigantic insects, tigers and lions."
			L" Tribes of humans that have denounced all types of technology also inhabits the surface."
		);

		m_normalTexts.push_back(
			L"For any species looking to build a new home planet or mine its resources, this planet is a perfect choice."
			L" While it currently holds very few lifeforms, life is certainly possible and the high amounts of precious resources make it the ideal place for many technological needs."
		);

		m_normalTexts.push_back(
			L"Resources are pretty much the only thing this planet is good for. Life will most likely never be sustained due to the harsh conditions and terra forming would be very difficult."
			L" But for any alien species advanced enough to mine other planets, this one would be a perfect choice."
		);

		m_normalTexts.push_back(
			L"Although diverse in colors, shapes and sizes, none of the organisms here have developed much."
			L" Most are still in a very crude form and those that did start to develop specialized parts have still a long way to go."
		);

		m_normalTexts.push_back(
			L"The planet offers a unique sight in the night sky of nearby planets."
			L" Its colors and distance to the planets around it occasionally makes the night quite magical on those planets, especially those with an atmosphere."
		);

		m_normalTexts.push_back(
			L"A few dozen insect species are the only life forms on this planet visible with the naked eye, but microscopes will show there's a huge variety of bacteria and other microorganisms hiding in every nook and cranny."
		);

		m_normalTexts.push_back(
			L"The mountainous parts of this planet make it quite unique compared to the rest of the solar system, formed over thousands of years thanks to the weathere conditions."
		);

		m_normalTexts.push_back(
			L"Water, water and more water. You've never seen this much blue in your whole life."
			L" There's only a few pieces of land for you to explore, so this shouldn't take long."
		);

		m_normalTexts.push_back(
			L"A single large capital, completely in ruins. Just the city itself would take several human lifetimes to explore."
			L" What could have ended a civilization as grand as this?"
		);
	}

	virtual ~FlavourTextManager() = default;

	std::wstring GetText(planetType type) {
		using t_clock = std::chrono::high_resolution_clock;
		std::default_random_engine generator(static_cast<UINT>(t_clock::now().time_since_epoch().count()));
		UINT textNr;
		std::wstring returnText;
		switch (type) {
		case planetType::warm:
		{
			std::uniform_int_distribution<UINT> distributionText(0, static_cast<UINT>(m_warmTexts.size()) - 1);
			textNr = distributionText(generator);
			returnText = m_warmTexts[textNr];
			m_warmTexts.erase(m_warmTexts.begin() + textNr);
			return returnText;
		}
		case planetType::cold:
		{
			std::uniform_int_distribution<UINT> distributionText(0, static_cast<UINT>(m_coldTexts.size()) - 1);
			textNr = distributionText(generator);
			returnText = m_coldTexts[textNr];
			m_coldTexts.erase(m_coldTexts.begin() + textNr);
			return returnText;
		}
		case planetType::radio:
		{
			std::uniform_int_distribution<UINT> distributionText(0, static_cast<UINT>(m_radioTexts.size()) - 1);
			textNr = distributionText(generator);
			returnText = m_radioTexts[textNr];
			m_radioTexts.erase(m_radioTexts.begin() + textNr);
			return returnText;
		}
		case planetType::normal:
		{
			std::uniform_int_distribution<UINT> distributionText(0, static_cast<UINT>(m_normalTexts.size()) - 1);
			textNr = distributionText(generator);
			returnText = m_normalTexts[textNr];
			m_normalTexts.erase(m_normalTexts.begin() + textNr);
			return returnText;
		}
		default:
			std::uniform_int_distribution<UINT> distributionText(0, static_cast<UINT>(m_normalTexts.size()) - 1);
			textNr = distributionText(generator);
			returnText = m_normalTexts[textNr];
			m_normalTexts.erase(m_normalTexts.begin() + textNr);
			return returnText;
		}
	}
};