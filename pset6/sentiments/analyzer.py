import nltk

class Analyzer():
    """Implements sentiment analysis."""
    pos_list = list()
    neg_list = list()
    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        positives = open(positives, "r")
        negatives = open(negatives, "r")
        for word in positives:
            if not word.startswith(';'):
                self.pos_list.append(word.strip())
        for word in negatives:
            if not word.startswith(';'):
                self.neg_list.append(word.strip())
        positives.close()
        negatives.close()
    
    def analyze(self, text):
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        score = 0
        for w in tokens:
            if w.lower() in self.pos_list:
                score += 1
            elif w.lower() in self.neg_list:
                score -= 1
            else:
                continue
        #if(score == 0):
            #print("Not found")
        return score