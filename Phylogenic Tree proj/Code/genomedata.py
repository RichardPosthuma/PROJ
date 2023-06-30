class GenomeData:
    def __init__(self, id) -> None:
        self._id = id
        self._sequence = ''
        self._ngrams = {}
    
    def add_sequence(self, seq):
        self._sequence += seq
    
    def add_ngrams(self, ngrams_set):
        self._ngrams = ngrams_set
    
        
    def sequence(self):
        return self._sequence
    
    def ngrams(self):
        return self._ngrams
    def id(self):
        return self._id
    
    def __repr__(self) -> str:
        return self._id