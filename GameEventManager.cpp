#include "pch.h"
#include "GameEventManager.h"


GameEvent GameEventManager::GetAGameEvent(UINT type)
{
	GameEvent gameEvent;

	switch (type)
	{
		case 0:
		{
			// Hot
			if (m_hotEventPool.empty()) {
				RefillGameEvents(type);
			}
			gameEvent = m_gameEvents[0][m_hotEventPool.back()];
			m_hotEventPool.pop_back();
			break;
		}
		case 1:
		{
			// Cold
			if (m_coldEventPool.empty()) {
				RefillGameEvents(type);
			}
			gameEvent = m_gameEvents[1][m_coldEventPool.back()];
			m_coldEventPool.pop_back();
			break;
		}
		case 2:
		{
			// Radioactive
			if (m_radioactiveEventPool.empty()) {
				RefillGameEvents(type);
			}
			gameEvent = m_gameEvents[2][m_radioactiveEventPool.back()];
			m_radioactiveEventPool.pop_back();
			break;
		}
		default:
		{
			// General
			if (m_generalEventPool.empty()) {
				RefillGameEvents(type);
			}
			gameEvent = m_gameEvents[3][m_generalEventPool.back()];
			m_generalEventPool.pop_back();
			break;
		}
	}

	return gameEvent;
}

void GameEventManager::RefillGameEvents(UINT type)
{
	switch (type) 
	{
		case 0: 
		{
			for (unsigned int i = 0; i < m_gameEvents[0].size(); i++) {
				m_hotEventPool.push_back(i);
			}
			std::random_shuffle(m_hotEventPool.begin(), m_hotEventPool.end());
			break;
		}
		case 1:
		{
			for (unsigned int i = 0; i < m_gameEvents[1].size(); i++) {
				m_coldEventPool.push_back(i);
			}
			std::random_shuffle(m_coldEventPool.begin(), m_coldEventPool.end());
			break;
		}
		case 2:
		{
			for (unsigned int i = 0; i < m_gameEvents[2].size(); i++) {
				m_radioactiveEventPool.push_back(i);
			}
			std::random_shuffle(m_radioactiveEventPool.begin(), m_radioactiveEventPool.end());
			break;
		}
		default:
		{
			for (unsigned int i = 0; i < m_gameEvents[3].size(); i++) {
				m_generalEventPool.push_back(i);
			}
			std::random_shuffle(m_generalEventPool.begin(), m_generalEventPool.end());
			break;
		}
	}
}

void GameEventManager::CreateGameEvents()
{

	std::vector<GameEvent> hotGameEvents = { // Hot Game Events
		{
			L"On the surface of the hot planet you find some sort of remnants from an ancient expedition."
			L" You notice some sort of gibberish on the side of a make-shift workbench, examine further?",

			L"Luckily you remember a few things from ancient languages-class and understand that it describes "
			L"something about a new type of propellant. You do not pay much attention to it though and just "
			L"grab a few cans of old fashioned fuel.",
			
			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{0, 30, 0, 0, 0, 0, 0, 0, 0, 1}
		},

		{
			L"After spending several hours scavenging the desert and ending up empty-handed you see a collection "
			L"of tents among the dunes. Waddle into the camp?",

			L"Within the camp you find a tribe of steampunk nomads, who have made it their lifestyle to endlessly "
			L"travel this landscape. You spend some time there, exchanging stories and provisions and exotic materials.",
			
			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{20, 0, -20, 0, 10, 10, 0, 0, 0, 1}
		},

		{
			L"When you land you see an old crash site in the distance, barely visible after being buried in the red sand. Investigate?",

			L"Investigating it further you see that the vessel is still intact and inside it you find some sort of propellant "
			L"that you have never seen before! Maybe it could be useful for something?",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{0, 0, 0, 0, 0, 0, 1, 0, 0, 1}
		},

		{
			L"After making your way to the lava lake on the planet that you saw from space you find a machine resembling an oil pump. "
			L"Try using the pump?",

			L"It is rusty, but still functional. It seems to be used to extract some special material from the lava lake. "
			L"After giving it a few cranks a purple-ish goo starts to flood out of the pump! Quickly you grab the sample canister "
			L"you brought with you to scoop up a good amount of the liquid before the pump is destroyed by the pressure.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{-20, 0, 0, 0, 0, 0, 1, 0, 0, 1}
		}
	};

	std::vector<GameEvent> coldGameEvents = { // Cold Game Events
		{
			L"While making your way through an icy storm you make out a transparent ice monolith randomly placed in this barren "
			L"landscape. Go closer? ",

			L"Getting closer to the monolith you start to make out faces within the ice. Terrifyingly you see miners, with "
			L"horrified expressions on their faces, within this everlasting tomb. Within their grasp you also notice some "
			L"weird glow coming from the rocks the miners seemed to be excavating. What could have created this tragedy?",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{0, 0, -30, 10, 0, 0, 0, 0, 0, 1}
		},

		{
			L"In the distance you make out something resembling a forest, upon closer inspection the trees are all frozen solid. "
			L"Explore the forest?",

			L"Making your way down a hill inside the forest you notice that as the altitude gets lower and lower, the temperature "
			L"gets higher and higher. Before you know it you are in a blooming forest greeted by a small man, seemingly older than "
			L"the forest itself. He helps you restock on supplies.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{40, 20, 20, 0, 0, 0, 0, 0, 0, 1}
		},

		{
			L"While exploring sub-surface caverns you stumble upon some kind of glowing frozen rock. Try to grab it?",

			L"When trying to remove it it explodes! The explosion adds another layer of ice to these already frozen caverns and to "
			L"your suit, luckily the ice shield that was crafted earlier for you and your ship protected you from an icy doom. ",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{0, 0, 0, 0, 0, 0, 0, 0, 1, 1}
		},

		{
			L"On the planet’s surface you see a sign indicating some kind of settlement. Follow the sign?",

			L"You find out that it is a research station! Inside are a couple of friendly researchers, "
			L"telling you that they are here to extract the renown mineral Khionerite. They tell you all about the story of the mineral, "
			L"how it made its way into the solar system as an asteroid and how many mining companies met their demise trying to extract "
			L"the rock. They wave you goodbye after giving you a sample of their findings.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{0, 0, 20, 0, 0, 0, 0, 0, 1, 1}
		}
	};

	std::vector<GameEvent> radioactiveGameEvents = { // Radioactive Game Events
		{
			L"Walking through the swamplands of the surface of this cursed planet you notice an old skeleton in armor, preserved in the "
			L"radioactive purple water. Investigate the remains?",

			L"After slipping when trying to take a closer look your suit rips below your elbow, the poisonous water starts pouring into "
			L"the suit. You grab a piece of the skeleton’s armor before hurrying back to the shuttle.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{-50, 0, 0, 20, 10, 0, 0, 0, 0, 1}
		},

		{
			L"A specific spot on the surface of this mountainous planet catches your eye, a perfectly circular open field that would be "
			L"a perfect spot to land. Change course and land there?",

			L"After landing you walk out of the shuttle into a barren landscape. Compared to the rest of this planet the outcrop is all black, "
			L"like it was burned. It’s the remnants of a battlefield, with the two factions being "
			L"mid-battle when this catastrophe struck. At the center is a crater, within is the remains of the nuke. "
			L"On the side you see a name in big green letters, “Shiro Hime”. You extract the powerful core of the nuke that is still partially intact.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{0, 0, 0, 0, 0, 10, 0, 1, 0, 1}
		},

		{
			L"When excavating the lush green radioactive forests you hear a deafening sound in the distance. Find out what made the noise?",

			L"You find yourself face to face with a green boar, having weird purple glowing spots all over its body. "
			L"You start running and zig-zag between the trees to try and escape the large beast. "
			L"You hear a scream behind you and then the sound of the boar retreating. "
			L"Turning around you see a piece of the glowing spots from the boar stuck in a sharp tree-branch. "
			L"You grab the glowing hide and make your way out of the forest.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{0, 15, -20, 0, 0, 0, 0, 1, 0, 1}
		},

		{
			L"Within the ruins of a great city you find a blocked off building with radioactive signs all over it. Try and remove the blockades?",

			L"Within is what is left of an old research lab, with a material similar to your own Radioactive Shield covering the walls to "
			L"either keep radioactivity from getting in, or from getting out. Everywhere around you there are smashed vials, it looks like "
			L"whatever they were working on here was sabotaged. You manage to at least salvage some materials from the destroyed lab.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{0, 0, 0, 10, 10, 10, 0, 0, 0, 1}
		}
	};

	std::vector<GameEvent> generalGameEvents = { // General Game Events
		{
			L"As you approach the planet you notice lots of debris in orbit. There is a lot of it so sifting through it will take hours but "
			L"you just might find something interesting. Explore?",

			L"You carefully navigate between meteorites. You see metallic shrapnel among the debris. "
			L"Suddenly you get rammed by a massive metallic structure. You freeze for a moment while the screeching sound of metal on metal seems to go on forever. "
			L"Your ship takes some serious damage. A part from the ship ‘The Anvil’ hit you. "
			L"You salvage what you can and get out of the field of debris.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{-40, 5, 0, 10, 15, 0, 0, 0, 0, 1}
		},

		{
			L"You stand at the edge of a massive crater. It’s quite a walk but there is something odd at the center that you think might be "
			L"worth investigating. Will you take a closer look?",

			L"You really underestimated that walk and the slope towards the bottom proved more treacherous than anticipated. After a tiring "
			L"trek and more than a few tumbles you close in on the oddity in the middle of the crater. A gleaming metallic object juts out of "
			L"the ground like a gigantic spear skewering the earth. As you get closer you see it used to be a part of a spaceship, ‘The Anvil’. "
			L"It really was worth the effort.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{-5, 0, -5, 15, 5, 0, 0, 0, 0, 1}
		},

		{
			L"Flying toward the surface of the planet you see an old and broken city. These places are often full of useful stuff. "
			L"Do you want to go there?",

			L"Your shuttle touches down in what used to be city downtown square. "
			L"Now the tall buildings are broken structures covered in plants. "
			L"As soon as you step outside you notice people eyeing you from windows and doorways. "
			L"A group of obvius warriors approach. When you tell them of your quest several of the younger ones ask to join you. "
			L"They bring some nice equipment along.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{40, 0, 0, 0, 0, 5, 0, 0, 0, 1}
		},

		{
			L"While exploring the planet you take a rest on a beach of unearthly beauty. While sitting on the emerald green sand you "
			L"contemplate a refreshing dip in the sparkling blue water. Go for a swim?",

			L"You undress and dive into the lake with childish abandon. While swimming and frolicking around in the refreshing waters "
			L"you suddenly realise that you are not alone. Mermaids! You stare at each other in wonder and spend the rest of the day "
			L"playing around. When time comes to leave the lake they give you many strange gifts from the deep. A few even turn out to "
			L"be useful. A refreshing adventure, indeed.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{25, 0, 0, 0, 5, 10, 0, 0, 0, 1}
		},

		{
			L"Descending into the planet’s atmosphere you had to dodge a lot of space-debris which looked to be from some kind of "
			L"large vessel. There also seems to be a lot of parts that have crashed into the planet. Explore the debris?",

			L"Walking around in this futuristic junkyard you see a metal plate stating: “PROPERTY OF THE SOLAR UNION - UNIT BABEL IX”. "
			L"You grab a few plates for the raw materials and hope that no one will miss them.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{0, 0, 0, 10, 10, 0, 0, 0, 0, 1}
		},

		{
			L"After opening the door of your shuttle you catch the weirdest smell. Your first reflex is to cover your nose in case "
			L"it is something toxic, but then you recognize it. The smell of freshly cooked meat?",

			L"Walking towards the smell you find something resembling a small village, but situated inside what is still left of a "
			L"crashed vessel. The people of the village are friendly, and you exchange stories. They tell you that their ancestors "
			L"crashed with this ship and that it was a part of the renowned space station Babel IX. They let you have the fuel that "
			L"is still left in the vessel as they do not have any use for it.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{30, 100, 0, 0, 0, 0, 0, 0, 0, 1}
		},

		{
			L"Strolling about on the planet you meet the most curious of creatures, a small goblin with an enormous backpack. "
			L"He proclaims to be the greatest tradesman ever known in the Solar Union. He tells you he is on the lookout for some "
			L"Nanotech, and will reward any adventurer that brings some to him very handsomely. Offer him some Nanotech?",

			L"When you bring out a few pieces of Nanotech from your own pockets you see his thoughts of riches on his facial expression "
			L"as clear as day. He quickly snatches the metal from your hands and throws you some other materials, undoubtedly in his mind "
			L"worth less than the Nanotech, before scurrying off.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{0, 0, 0, 10, 10, -10, 0, 0, 0, 1}
		},

		{
			L"In the middle of a ruined town you see two men in old soldier outfits having a heated argument, you hide behind some old fences. "
			L"One of the men suddenly brings his rifle down from his shoulder and aims at his comrade. Intervene?",

			L"Bandits! It turns out that they knew you were there all along and played this theatre to make you come out of hiding. They took "
			L"some materials you found earlier and left you bound to a pole and waved you goodbye saying “Viva la revolución!”. After a few "
			L"hours of struggle you manage to break loose and make it back to the shuttle. At least you got a few hours of rest while bound.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{30, 0, 30, -5, -5, -5, 0, 0, 0, 1}
		},

		{
			L"On your way down to the planet’s surface you see a few red flashing lights on your control panel. Do you want to try and find the problem when you land?",

			L"It turns out the shuttle’s motor was malfunctioning and required repairs, meanwhile it exposed you to extreme heat for a few minutes. "
			L"After having made the repairs you notice a hidden compartment in the back of the shuttle stashed with provisions!",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{-30, 50, 50, 0, -10, 20, 0, 0, 0, 1}
		},
			
		{
			L"Moments after having been launched from the ship you notice something odd about the asteroid belt that surrounds the planet. "
			L"Bathed in the light of the dying sun it sometimes seems to flicker and shine in odd ways. Explore the asteroid belt?",

			L"You engage manual controls and steer the probe off from it’s programmed path, towards the asteroid belt instead. "
			L"Small thuds against the hull of the probe tells you you’ve arrived at the belt, and that you must proceed with caution. "
			L"A quick scan of the asteroids shows you that they’re mixed with Nanotech. "
			L"The extraction part, while fruitful, takes a painstakingly long time.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{0, 0, -50, 0, 0, 30, 0, 0, 0, 1}
		},

		{
			L"Having arrived at the planet, you notice that it is orbited by not one, but two moons. "
			L"Your approach vector to the planet is aligned with the bigger of the two moons as well, and you wonder what discoveries could be made on its surface. Explore the moon?",

			L"You touch down on the moon and begin the exploration. The white surface shines bright in your eyes as you gaze about the surroundings. "
			L"You decide to step into a cave not far from your original position, where you stumble upon a hastily abandoned small camp. "
			L"As you gather what materials you can find you wonder what made the owners leave in such a hurry…",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{0, 0, 0, 1, 10, 1, 0, 0, 0, 1}
		},

		{
			L"As you round a bend you suddenly find yourself face to face with a massive stone formation, with two stone statues placed on either side of its narrow entrance. "
			L"The statues have the form of cloaked shadow figures, their silhouettes shrouded but for their hands, stretched outwards towards you with open palms. "
			L"The whole scene feels like a display of discouragement. Enter?",

			L"You quickly realize entering the stone formation was a bad idea. " 
			L"It is a maze, twisting and turning about every direction, with every new location feeling both familiar and foreign at the same time. "
			L"You also come to feel the ominous feeling of being watched, eyes prying as soon as you turn your back. "
			L"You make it out after what feels like a lifetime, and with little to show for it.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{0, 0, -40, 2, 10, 0, 0, 0, 0, 1}
		},

		{
			L"As you make camp for the night you are disrupted by the sound of a soft voice from a woman, singing a sad tune. "
			L"You can barely make it out over the sounds of the wind, but you are sure it is there. "
			L"You also notice something else, the faint smell of smoke. Make your way towards the source of the singing?",

			L"You enter a large glade in a forest area, the singing voice is now as clear as the overgrown ruins of the small town that stands before you. "
			L"When you reach the center of the town you see the source of the singing before you, a woman in rough garbs holding her small baby in her arms. "
			L"She looks up towards you with a sad expression. Painted on a wall of a building are the crossed out letters “SU”.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{0, 20, 0, 0, 20, 5, 0, 0, 0, 1}
		},

		{
			L"As you make your way through a dale you spot a trading caravan in the distance, a combination of humanoids and their pack beasts preparing to set up camp for the day. "
			L"One of the members notices you and signals for you to join them. "
			L"You wonder what goods they might offer, as you ponder on whether to accept the invitation. Join the caravan?",

			L"As you spend the evening with the caravan they tell you they are the remnants of a trading empire belonging to the so called “Solar Union”, a vast network of planets united under a common banner in order to uphold peace in the solar system. "
			L"Yet it had inevitably failed, and war had ensued. "
			L"What remains are scattered political units all vying for control of the SU, and the bloody battles that follow.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{0, 30, 0, 5, 10, 5, 0, 0, 0, 1}
		},

		{
			L"While exploring the planet you chance upon a small fort of some kind. "
			L"You spot fuel cells but there are a lot of guards. Wait for an opportunity to snatch some?",

			L"At sunset something happens in the encampment and the opportunity you’ve been waiting for presents itself. "
			L"Crouching low, you dash from cover to cover. By the fuel cells there is what seems like an advanced radar and it has an important component you need. "
			L"You take it and grab some fuel cells and run.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{0, 25, 0, 0, 0, 5, 0, 0, 0, 1}
		},

		{
			L"A line of soldiers trail out of the forest ahead. You swiftly duck into cover but, alas, too late. "
			L"While running toward your shuttle you spot a mysterious cave. Hide there instead?",

			L"Wrong choice! There is military equipment all over the place. The soldiers enter and capture you. "
			L"After interrogation and a solid beating they throw you in prison. Another prisoner, the scientist Irma Govnorova, tends to your wounds. "
			L"During that night you find a way to escape together. For good measure you steal some advanced equipment on your way out.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{-25, 0, -5, 0, 0, 5, 0, 0, 0, 1}
		},

		{
			L"As you come to the top of the hill you hear the sounds of fighting. Soldiers are raiding a village ahead. "
			L"You see a way to intervene and help the villagers. Meddle in the violence unfolding?",

			L"Sneaking around you take out the soldiers in the armored vehicle with your can of sleeping gas. "
			L"You drive ahead and after a few warning shots the rest of the soldiers surrender. "
			L"The villagers are overjoyed but fear the return of more soldiers. You have room on the ship and take them with you.",

			Resources{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			Resources{25, 0, 0, 5, 15, 0, 0, 0, 0, 1}
		}
	};

	m_gameEvents.push_back(hotGameEvents);
	m_gameEvents.push_back(coldGameEvents);
	m_gameEvents.push_back(radioactiveGameEvents);
	m_gameEvents.push_back(generalGameEvents);
}



GameEventManager::GameEventManager()
{
	CreateGameEvents();

	for (unsigned int i = 0; i < m_gameEvents[0].size(); i++) {
		m_hotEventPool.push_back(i);
	}
	for (unsigned int i = 0; i < m_gameEvents[1].size(); i++) {
		m_coldEventPool.push_back(i);
	}
	for (unsigned int i = 0; i < m_gameEvents[2].size(); i++) {
		m_radioactiveEventPool.push_back(i);
	}
	for (unsigned int i = 0; i < m_gameEvents[3].size(); i++) {
		m_generalEventPool.push_back(i);
	}

	srand((unsigned int)time(NULL));
	std::random_shuffle(m_generalEventPool.begin(), m_generalEventPool.end());
	std::random_shuffle(m_coldEventPool.begin(), m_coldEventPool.end());
	std::random_shuffle(m_hotEventPool.begin(), m_hotEventPool.end());
	std::random_shuffle(m_radioactiveEventPool.begin(), m_radioactiveEventPool.end());
}

GameEventManager::~GameEventManager()
{
}

void GameEventManager::RequestGameEvents(GameEvent setOfGameEvents[3], UINT planetType)
{
	// Guarantee that one game event is of given type
	setOfGameEvents[0] = GetAGameEvent(planetType);

	// Randomize if if the remaining game events should be general or planetType
	bool rnd = 0;
	int type = 2;
	for (int i = 1; i < 3; i++) {
		rnd = rand() % 2;

		if (rnd) type = planetType;
		else  type = 3;
		setOfGameEvents[i] = GetAGameEvent(type);
	}
}
