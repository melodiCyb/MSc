

#Word frequency counting with Binary Tree

import string
import re

# Each object stores a tuple: (count,word)
class Binary_Tree_Node:
    left = None
    right = None
    data = ()
    def __init__(self,word):
        self.data = (1,word)
        
    #Increment the count for the word.
    #Create the counter if new word is encountered.
    def increment(self,word): 

        if word < self.data[1]:
            
            #If there nothing on the left
            if self.left == None:
                self.left = Binary_Tree_Node(word)
                
            #Else there is a left *subtree*   
            else:
                self.left.increment(word)
                
        elif word > self.data[1]:
            
            #If there is nothing on the right
            if self.right == None: 
                self.right = Binary_Tree_Node(word)
                
            #There is a right *subtree*
            else:
                self.right.increment(word)
        else: 
            # word == self.data[1]
            #Increment the count
            self.data = (self.data[0] + 1,self.data[1])

    def get(self): 
        #returns a list in alphbetical order
        result = []
        if self.left != None:
            result = self.left.get()
        result = result + [self.data]
        if self.right !=None:
            result = result + self.right.get()
        return result

#Create the binary tree
#Root will be a dummy node
tree = Binary_Tree_Node(" ") 

with open('Hamlet.txt') as f:
    text = f.readlines()
    
# remove whitespace
text = [x.strip() for x in text]

for line in text: 
    #build the tree
    group = line.split()
    for word in group:
        #remove non-alphabetical characters from words
        #and make them all lowercase
        #to avoid ambiguities
        word=re.sub(r'[^\w\s]','',word)
        tree.increment(word.lower())
        #call increment()
        
#save tree in a list
result = tree.get() 

#remove the dummy node
result = result[1:]

#sort the result
result.sort()

#print the results
for pair in result: 
    print "%d\t%s" % pair

