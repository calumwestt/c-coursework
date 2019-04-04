I created this program using SDL to try and experiment with other built-in
functions that come with SDL. I had trouble using SDL Image so ended up
resorting to just using squares/rectangles for characters in the game.

I struggled with segfaults alot during the making of the game and was therefore
unable to include more than 4 asteroids at a time for some reason. I think the
issue is to do with how the main game loop executes so quickly that updating
every detail about each asteroid causes an overflow. Not really sure of the
real reason though and is probably a human error somewhere on my part.

The aim of the game is for you, the ship (white square), to avoid the asteroids
(red rectangles). You can do this by using the arrow keys to move, these are the
only controls. If you collide with one, the game is over and you die, you
can run the game again and play as many times as you like.

To further improve the game I would like to add features such as increasing the
number of asteroids as time goes on to increase the difficulty of the game and
possibly include a timer that displays after you die so you can see how long
you survived.
