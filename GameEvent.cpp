#include "pch.h"
#include "GameEvent.h"


std::vector< std::vector<GameEvent> > gameEvents = {

   { // Hot Game Events
		{
		L"On the surface of the hot planet you find some sort of remnants from an ancient expedition."
		L" You notice some sort of gibberish on the side of a make-shift workbench, examine further?",
		
		L"Luckily you remember a few things from ancient languages-class and understand that it describes "
		L"something about a new type of propellant. You do not pay much attention to it though and just "
		L"grab a few cans of old fashioned fuel.",

		{30, 0, 0, 0, 0, 0, 0, 0}, 0
		},

		{
		L"After spending several hours scavenging the desert and ending up empty-handed you see a collection "
		L"of tents among the dunes. Waddle into the camp?",

		L"Within the camp you find a tribe of steampunk nomads, who have made it their lifestyle to endlessly "
		L"travel this landscape. You spend some time there, exchanging stories and provisions and exotic materials.",
		
		{0, -20, 0, 10, 10, 0, 0, 0}, 20
		},

		{
		L"When you land you see an old crash site in the distance, barely visible after being buried in the red sand. Investigate?",
		
		L"Investigating it further you see that the vessel is still intact and inside it you find some sort of propellant "
		L"that you have never seen before! Maybe it could be useful for something?",
		
		{0, 0, 0, 0, 0, 1, 0, 0}, 0
		},

		{
		L"After making your way to the lava lake on the planet that you saw from space you find a machine resembling an oil pump. "
		L"Try using the pump?",
		
		L"It is rusty, but still functional. It seems to be used to extract some special material from the lava lake. "
		L"After giving it a few cranks a purple-ish goo starts to flood out of the pump! Quickly you grab the sample canister "
		L"you brought with you to scoop up a good amount of the liquid before the pump is destroyed by the pressure.",
		
		{0, 0, 0, 0, 0, 1, 0, 0}, -20
		}
   },


   { // Cold Game Events
		{
		L"While making your way through an icy storm you make out a transparent ice monolith randomly placed in this barren "
		L"landscape. Go closer? ",

		L"Getting closer to the monolith you start to make out faces within the ice. Terrifyingly you see miners, with "
		L"horrified expressions on their faces, within this everlasting tomb. Within their grasp you also notice some "
		L"weird glow coming from the rocks the miners seemed to be excavating. What could have created this tragedy?",

		{0, -30, 10, 0, 0, 0, 0, 0}, 0
		},

		{
		L"In the distance you make out something resembling a forest, upon closer inspection the trees are all frozen solid. "
		L"Explore the forest?",

		L"Making your way down a hill inside the forest you notice that as the altitude gets lower and lower, the temperature "
		L"gets higher and higher. Before you know it you are in a blooming forest greeted by a small man, seemingly older than "
		L"the forest itself. He helps you restock on supplies.",

		{20, 20, 0, 0, 0, 0, 0, 0}, 40
		},

		{
		L"While exploring sub-surface caverns you stumble upon some kind of glowing frozen rock. Try to grab it?",

		L"When trying to remove it it explodes! The explosion adds another layer of ice to these already frozen caverns and to "
		L"your suit, luckily the ice shield that was crafted earlier for you and your ship protected you from an icy doom. ",

		{0, 0, 0, 0, 0, 0, 0, 1}, 0
		},

		{
		L"On the planet’s surface you see a sign indicating some kind of settlement. Follow the sign?",

		L"After having made your way there you find out that it is a research station! Inside are a couple of friendly researchers, "
		L"telling you that they are here to extract the renown mineral Khionerite. They tell you all about the story of the mineral, "
		L"how it made its way into the solar system as an asteroid and how many mining companies met their demise trying to extract "
		L"the rock. With your newfound knowledge they wave you goodbye after giving you a sample of their findings.",

		{0, 20, 0, 0, 0, 0, 0, 1}, 0
		}
   },


   { // Radioactive Game Events
		{
		L"Walking through the swamplands of the surface of this cursed planet you notice an old skeleton in armor, preserved in the "
		L"radioactive purple water. Investigate the remains?",

		L"After slipping when trying to take a closer look your suit rips below your elbow, the poisonous water starts pouring into "
		L"the suit. You grab a piece of the skeleton’s armor before hurrying back to the shuttle.",

		{0, 0, 20, 10, 0, 0, 0, 0}, -50
		},

		{
		L"A specific spot on the surface of this mountainous planet catches your eye, a perfectly circular open field that would be "
		L"a perfect spot to land. Change course and land there?",

		L"After landing you walk out of the shuttle into a barren landscape. Compared to the rest of this green and purple planet "
		L"this outcrop is all black, like it was burned. It appears to be the remnants of a battlefield, with the two factions being "
		L"mid-battle when this catastrophe struck. At the center is a crater, within is the remains of the nuke that must have been "
		L"the cause of this. On the side you see a name in big green letters, “Shiro Hime”. You extract the powerful core of the nuke "
		L"that is still partially intact.",

		{0, 0, 0, 0, 10, 0, 1, 0}, 0
		},

		{
		L"When excavating the lush green radioactive forests you hear a deafening sound in the distance. Find out what made the noise?",

		L"After struggling through thick vines you find yourself face to face with a 3 meter tall green boar, having weird purple "
		L"glowing spots all over its body. You must have stumbled into its territory, you decide that you have the best chance of "
		L"survival if you try to escape. You start running and zig-zag between the trees to try and escape the large beast. You hear "
		L"a scream behind you and then the sound of the boar retreating. Turning around you see a pool of green blood on the ground "
		L"and a piece of the glowing spots stuck in a sharp tree-branch. You grab the glowing hide which feels more like a rock and "
		L"make your way out of the forest.",

		{15, -20, 0, 0, 0, 0, 1, 0}, 0
		},

		{
		L"Within the ruins of a great city you find a blocked off building with radioactive signs all over it. Try and remove the blockades?",

		L"Within is what is left of an old research lab, with a material similar to your own Radioactive Shield covering the walls to "
		L"either keep radioactivity from getting in, or from getting out. Everywhere around you there are smashed vials, it looks like "
		L"whatever they were working on here was sabotaged. You manage to at least salvage some materials from the destroyed lab.",

		{0, 0, 10, 10, 10, 0, 0, 0}, 0
		}
   },

   { // General Game Events
		{
		L"As you approach the planet you notice lots of debris in orbit. There is a lot of it so sifting through it will take hours but "
		L"you just might find something interesting. Explore?",

		L"Entering the field of debris you carefully navigate between meteorites, small and large. You also see metallic shrapnel among "
		L"the debris. This was probably the result of an impact between a spaceship and an asteroid. Coming around a huge meteorite you "
		L"suddenly get rammed by a massive metallic structure. For a moment you freeze in your chair while the screeching sound of metal "
		L"on metal seems to go on forever. Your ship takes some serious damage but luckily nothing critical. A part from the ship "
		L"‘The Anvil’ hit you. You salvage what you can and get out of the field of debris.",

		{5, 0, 10, 15, 0, 5, 0, 0}, -40
		},

		{
		L"You stand at the edge of a massive crater. It’s quite a walk but there is something odd at the center that you think might be "
		L"worth investigating. Will you take a closer look?",

		L"You really underestimated that walk and the slope towards the bottom proved more treacherous than anticipated. After a tiring "
		L"trek and more than a few tumbles you close in on the oddity in the middle of the crater. A gleaming metallic object juts out of "
		L"the ground like a gigantic spear skewering the earth. As you get closer you see it used to be a part of a spaceship, ‘The Anvil’. "
		L"It really was worth the effort.",

		{0, -5, 15, 5, 0, 0, 0, 0}, -5
		},

		{
		L"Flying toward the surface of the planet you see an old and broken city. These places are often full of useful stuff. "
		L"Do you want to go there?",

		L"Your shuttle touches down in what used to be city downtown square. Now the tall buildings are broken structures covered "
		L"in plants and the asphalt underneath your feet is broken and pierced by trees and their roots. As soon as you step outside "
		L"you notice people eying you suspiciously from windows and doorways. A group of obvius warriors approach. However, when you "
		L"make the peace sign they relax visibly. Soon you are laughing and dining together. When you tell them of your quest several of "
		L"the younger ones ask to join you. They bring some nice equipment along.",

		{0, 0, 0, 0, 5, 0, 0, 0}, 40
		},

		{
		L"While exploring the planet you take a rest on a beach of unearthly beauty. While sitting on the emerald green sand you "
		L"contemplate a refreshing dip in the sparkling blue water. Go for a swim?",
		
		L"You undress and dive into the lake with childish abandon. While swimming and frolicking around in the refreshing waters "
		L"you suddenly realise that you are not alone. Mermaids! You stare at each other in wonder and spend the rest of the day "
		L"playing around. When time comes to leave the lake they give you many strange gifts from the deep. A few even turn out to "
		L"be useful. A refreshing adventure, indeed.",
		{0, 0, 0, 5, 10, 0, 0, 0}, 25
		},

		{
		L"Descending into the planet’s atmosphere you had to dodge a lot of space-debris which looked to be from some kind of "
		L"large vessel. There also seems to be a lot of parts that have crashed into the planet. Explore the debris?",

		L"Walking around in this futuristic junkyard you see a metal plate stating: “PROPERTY OF THE SOLAR UNION - UNIT BABEL IX”. "
		L"You grab a few plates for the raw materials and hope that no one will miss them.",

		{0, 0, 10, 10, 0, 0, 0, 0}, 0
		},

		{
		L"After opening the door of your shuttle you catch the weirdest smell. Your first reflex is to cover your nose in case "
		L"it is something toxic, but then you recognize it. The smell of freshly cooked meat?",

		L"Walking towards the smell you find something resembling a small village, but situated inside what is still left of a "
		L"crashed vessel. The people of the village are friendly, and you exchange stories. They tell you that their ancestors "
		L"crashed with this ship and that it was a part of the renowned space station Babel IX. They let you have the fuel that "
		L"is still left in the vessel as they do not have any use for it.",

		{100, 0, 0, 0, 0, 0, 0, 0}, 30
		},

		{
		L"Strolling about on the planet you meet the most curious of creatures, a small goblin with an enormous backpack. "
		L"He proclaims to be the greatest tradesman ever known in the Solar Union. He tells you he is on the lookout for some "
		L"Nanotech, and will reward any adventurer that brings some to him very handsomely. Offer him some Nanotech?",

		L"When you bring out a few pieces of Nanotech from your own pockets you see his thoughts of riches on his facial expression "
		L"as clear as day. He quickly snatches the metal from your hands and throws you some other materials, undoubtedly in his mind "
		L"worth less than the Nanotech, before scurrying off.",

		{0, 0, 10, 10, -10, 0, 0, 0}, 0
		},

		{
		L"In the middle of a ruined town you see two men in old soldier outfits having a heated argument, you hide behind some old fences. "
		L"One of the men suddenly brings his rifle down from his shoulder and aims at his comrade. Intervene?",

		L"Bandits! It turns out that they knew you were there all along and played this theatre to make you come out of hiding. They took "
		L"some materials you found earlier and left you bound to a pole and waved you goodbye saying “Viva la revolución!”. After a few "
		L"hours of struggle you manage to break loose and make it back to the shuttle. At least you got a few hours of rest while bound.",
		
		{0, 30, -5, -5, -5, 0, 0, 0}, 30
		}
   }
};

GameEvent GetGameEvent(UINT ID)
{
	UINT type = (ID >> 8) & 0xFF;
	UINT index = ID & 0xFF;

	if (type > 3) {
		type = 3;
	}

	return gameEvents[type][index];
}

UINT CreateID(UINT type, UINT index)
{
	return ((type << 8) & 0xFF00) + index;
}
