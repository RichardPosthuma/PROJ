
class Tree:
    def __init__(self, id=None, leaf=True) -> None:
        self._id = id
        self._rchild = None
        self._lchild = None
        self._leaf = leaf
        if leaf:
            self._children = None
        else:
            self._children = []
    
    def id(self):
        return self._id
    
    def rchild(self):
        return self._rchild
    
    def lchild(self):
        return self._lchild
    
    def set_right(self, value):
        self._rchild = value
        if value.id() != None:
            self._children.append(value)
        
    def set_left(self, value):
        self._lchild = value
        if value.id() != None:
            self._children.append(value)
    
    def children_list(self):
        return self._children
    
    def set_children(self, value:list):
        self._children = value
    def leaf(self):
        return self._leaf
    
    def __repr__(self) -> str:
        if self._id == None:
            return "None"
        return self._id.id()
    def __str__(self):
        if self.leaf():
            return self.id().id()
        else:
            return "({}, {})".format(str(self.lchild()), str(self.rchild()))