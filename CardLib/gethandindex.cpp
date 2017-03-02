size_t
getHandIndex(const CardVector_t& cards)

{

    size_t handIndex = 0;

 

    size_t cardCount = cards.size();

    size_t highestCardForIndex = DeckSize - 1;

    for (size_t cardIndex = 0; cardIndex < cardCount; ++cardIndex)

    {

        size_t remainingCardsInHand = cardCount - cardIndex - 1;

        if (remainingCardsInHand > 0)

        {

            Card_t::Value_t cardValue = (Card_t::Value_t)highestCardForIndex;

            for (; cardValue > cards[cardIndex].getValue(); --cardValue)

            {

                if (remainingCardsInHand > 1)

                {

                    size_t C = cardValue;

                    size_t K = remainingCardsInHand;

                    

                    size_t combos = combinations(C, K);

                    handIndex += combos;

                }

                else

                {

                    handIndex += cardValue;

                }

            }

        }

        else

        {

            //printf("before last card handIndex(%ld) highestCardForIndex(%ld)\n", handIndex, highestCardForIndex);

            handIndex += highestCardForIndex - cards[cardIndex].getValue();

        }

        printf ("cardIndex(%d) handIndex(%d)\n", cardIndex, handIndex);

        highestCardForIndex = cards[cardIndex].getValue() - 1;

    }

    return handIndex;

}
