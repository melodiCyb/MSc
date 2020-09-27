
class Stack:
    def __init__(self):
        self.elements = []
        
    def isEmpty(self):
        return self.elements == []

    def push(self, element):
        self.elements.append(element)
        
    def pop(self):
         return self.elements.pop()
    
    def look(self):
        return self.elements[-1]
    

def matching(input_string):
    stack=Stack()
    delimiters = {'(': ')', '[': ']', '{': '}'}
    for i in input_string:
        if i in delimiters.keys():
            stack.push(i)
        elif i in delimiters.values():  
            if stack.isEmpty():
                return "not correct", i, "does not have an opening"
            elif delimiters[stack.look()] == i:  
                stack.pop()
            else:
                print "not correct"
                return stack.look(), "does not match",i
    if not stack.isEmpty():
        return "incorrect"
    else:
        return "correct"


class Node(object):
    
    def __init__(self, value, prev, next):
        self.value = value
        self.prev = prev
        self.next = next
    
    def get_value(self):
        return self.value


    def get_next(self):
        return self.next
    

    def get_prev(self):
        return self.prev


class DoublyLinkedList(object):
    
    head=tail = None
    
    def insert(self, value): 
        newNode = Node(value, None, None) 
        if self.head is None:
            self.head = self.tail = newNode
        else:
            newNode.prev = self.tail
            newNode.next = None
            self.tail.next = newNode
            self.tail = newNode
    
    def delete(self, node_value):
        currentNode = self.head
        
        while currentNode is not None:
            if currentNode.value == node_value:
                if currentNode.prev is not None:
                    currentNode.prev.next = currentNode.next
                    currentNode.next.prev = currentNode.prev
                else:
                    self.head = currentNode.next
                    currentNode.next.prev = None
            currentNode = currentNode.next
    
    def find(self, value):
        current = self.head
        found = False
        while current and found is False:
            if current.get_value() == value:
                found = True
            else:
                current = current.get_next()
        if current is None:
            print "Node with value " , value , " is not found"
        else:
            print "Node with value " , value,  " is found"
            
    def traverseForward(self):
        currentNode = self.head
        while currentNode != None:
            print currentNode.get_value() 
            currentNode = currentNode.next
            
    def traverseBackward(self):
        currentNode = self.tail
        while currentNode != None:
            print currentNode.get_value()
            currentNode = currentNode.prev


