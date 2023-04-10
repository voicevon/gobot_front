# This is a list, not a array.

# When should I use list, not array?
- If we can use an object array, do not use a list, array is simpler.
- But, sometimes the object is not written by ourself.
 - And that objetct CAN NOT BE CONSTRUCTED WITH BLANK/EMPTY ARGUS.
  - Bucause, there is no blank constructor.
 - Saying:
  - 1. We can not delcare an 'default' object.
  - 2. The only way to init the object, is when it's being constructed.
- When we meet this sinario, The array is not suitable, we have to consider list.

# Init process
1. Create many objects
 - Create objects one by one, with different constructor argus.
2. Create an list pointer.
3. Add the objects to the list one by one.