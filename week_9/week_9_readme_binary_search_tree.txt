Binary Search Tree - Calum West
I created this program following a suggestion made by you at the end of the lists program page. The program involves a traversal of the binary search tree in 3 different ways:
Pre-order
In-order
Post-order
These different traversal techniques were much simpler in implementation than I imagined and were very similar to each other in the way that they worked. I had some trouble free-ing the tree at the end of the program to prevent memory leaks but figured out a way by using one of the traversals I had made earlier and calling free at each node. The majority of functions in the program are recursive and wrap themselves up nicely when they all unwind.
I found that writing tests for this program was difficult so I kept them to a minimum and tested each traversal myself by writing drawing out the tree and checking the traversals were printed in the correct order. Other means of testing were simply to check that there were no memory leaks in the program.
I think to improve the program I would add the ability to add nodes one at a time from the terminal and have the option to delete/add more nodes once you have initially run the program. I would also like to add the ability to display the tree in a way that shows it as a tree.
To run the program with the singular test you can type './binary_search_tree'
If you would like to create your own tree you can run the program from terminal with arguments that you would like to be in your nodes in the order of insertion e.g './binary_search_tree 5 8 1 3 9 7 4 2 6' would insert the values in order of '5, 8, 1, 3, 9, 7, 4, 2, 6'.
