#include "pch.h"
#include "GameEvent.h"


std::vector< std::vector<GameEvent> > gameEvents = {

   { // Hot Game Events
	   {
	   "Test 1",
	   "result",
	   {0, 0, 0, 0, 0, 5, 0, 0}
	   },
	   {
	   "Test 2",
	   "result",
	   {10, 5, 0, 0, 0, 1, 0, 0}
	   },
	   {
	   "Test 3",
	   "result",
	   {100, 100, 100, 100, 100, 10, 10, 10}
	   },
	   {
	   "Test 4",
	   "result",
	   {100, -100, 100, 0, 0, 5, 0, 0}
	   }
   },


   { // Cold Game Events
	   {
	   "Test 5",
	   "result",
	   {0, 0, 0, 0, 0, 0, 0, 5}
	   },
	   {
	   "Test 6",
	   "result",
	   {10, 5, 0, 0, 0, 0, 0, 1}
	   },
	   {
	   "Test 7",
	   "result",
	   {100, 100, 100, 100, 100, 10, 10, 10}
	   },
	   {
	   "Test 8",
	   "result",
	   {100, -100, 100, 0, 0, 0, 0, 5}
	   }
   },


   { // Radioactive Game Events
	   {
	   "Test 9",
	   "result",
	   {0, 0, 0, 0, 0, 0, 0, 5}
	   },
	   {
	   "Test 10",
	   "result",
	   {10, 5, 0, 0, 0, 0, 0, 1}
	   },
	   {
	   "Test 11",
	   "result",
	   {100, 100, 100, 100, 100, 10, 10, 10}
	   },
	   {
	   "Test 12",
	   "result",
	   {100, -100, 100, 0, 0, 0, 0, 5}
	   }
   },

   { // General Game Events
	   {
	   "Now this is the story all about how My life got flipped, turned upside down",
	   "In west Philadelphia born and raised On the playground where I spent most of my days Chilling out, maxing, relaxing all cool",
	   {10, 0, 0, 0, 0, 5, 0, 0}
	   },
	   {
	   "Test",
	   "Bad choice",
	   {10, 5, -10, 0, 0, 0, 0, -1}
	   },
	   {
	   "Raid: Shadow Legends",
	   "You got a great sponsorship that rewarded you with plenty of resources.",
	   {100, 100, 100, 100, 100, 10, 10, 10}
	   },
	   {
	   "Prevent wildfires",
	   "Smokey The Bear liked that",
	   {100, -100, 10, 0, 0, -2, 0, 0}
	   }
   }
};