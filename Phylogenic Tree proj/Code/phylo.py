'''
    File: phylo.py
    Author: Richard Posthuma
    Purpose: given a fasta file of different 
    strand of some virus, we look at each dna
    sequence and compaire their simmilarity.
    using this simmilarity, we use a binary tree to 
    create a phylogenetic tree
'''

from genomedata import *  # contains genome class
from tree import *  # contains tree class

def proccess_data(file:list) -> list:
    """takes in a list with data from a fasta file. 
    we then itterate though it and create an instance of 
    the GenomeData() class every time we come across a new strain
    and add its assosiated DNA sequence. we then add that instance 
    to a list and return said list
    
    Note: bc we increment i by one every time we come across a new
    strain, we initialize i at -1 bc the first line of the fasta file
    will be a strain
    Args:
        file (list): a list with data from a fasta file

    Returns:
        list: a list of GenomeData class instances 
    """
    genome_lst = []
    i = -1
    prev_space = 1
    for line in file:
        line = line.strip('\n')
        if prev_space == 1 and line != '':
            line = line
            line = line.lstrip('>').split()[0]
            genome_lst.append(GenomeData(line))  #
            i += 1
            prev_space = 0
        else:
            if line == '':
                prev_space = 1
            else:
                genome_lst[i].add_sequence(line)
    return genome_lst

def genome_nrgam(genome_lst:list, n_size:int) -> list:
    """

    Args:
        genome_lst (list): _description_
        n_size (int): _description_

    Returns:
        list: _description_
    """
    for i in range(len(genome_lst)):
        genome_lst[i].add_ngrams(get_ngrams(genome_lst[i].sequence(), n_size))
    return genome_lst

def seq_set_sim(s1, s2):
    return float(len(s1.intersection(s2)))/float(len(s1.union(s2)))

def create_tree(tree_list, n):
    while len(tree_list) > 1:
        max_sim = [0, None, None, None]
        for node1 in tree_list:
            for node2 in tree_list:
                if node1 != node2:
                    if node1.id() == None and node2.id() == None:
                        for child1 in node1.children_list():
                            for child2 in node2.children_list():
                                x = seq_set_sim(child1.id().ngrams(), child2.id().ngrams())
                                if  x > max_sim[0]:
                                    max_sim = [x, child1, child2, node1, node2]
                    elif node1.id() == None:
                        for child in node1.children_list():
                            x = seq_set_sim(child.id().ngrams(), node2.id().ngrams())
                            if x > max_sim[0]:
                                max_sim = [x, child, node2, node1]
                    elif node2.id() == None:
                        for child in node2.children_list():
                            x = seq_set_sim(child.id().ngrams(), node1.id().ngrams())
                            if x > max_sim[0]:
                                max_sim = [x, child, node1, node2]
                    else:
                        x = seq_set_sim(node1.id().ngrams(), node2.id().ngrams())
                        if x > max_sim[0]:
                            max_sim = [x, node1, node2]
        new_node = Tree(None, False)
        if len(max_sim) == 5:  # when sim between two parents
            if str(max_sim[3]) > str(max_sim[4]):
                l_child, r_child = max_sim[4], max_sim[3]
            else:
                l_child, r_child = max_sim[3], max_sim[4]    
            new_node.set_children(max_sim[3].children_list() + max_sim[4].children_list()) 
            tree_list.remove(max_sim[3])
            tree_list.remove(max_sim[4])
        elif len(max_sim) == 4:  # when sim between parent and leaf
            if str(max_sim[2]) > str(max_sim[3]):
                l_child, r_child = max_sim[3], max_sim[2]
            else:
                l_child, r_child = max_sim[2], max_sim[3]
            if max_sim[3].id():
                new_node.set_children(max_sim[2].children_list())
            else:
                new_node.set_children(max_sim[3].children_list()) 
            tree_list.remove(max_sim[3])
            tree_list.remove(max_sim[2])
        else:  # when sim between leaf and leaf
            if str(max_sim[1]) > str(max_sim[2]):
                l_child, r_child = max_sim[2], max_sim[1]
            else:
                l_child, r_child = max_sim[1], max_sim[2]
            tree_list.remove(max_sim[1])
            tree_list.remove(max_sim[2])
        tree_list.append(new_node)
        new_node.set_left(l_child)
        new_node.set_right(r_child)
    return tree_list
            
def get_ngrams(seq, n):
    return {seq[i:i+n] for i in range(len(seq)-n)}

def main():
    file_name = input("FASTA file: ")
    fasta_file = open(file_name, 'r').readlines()
    n_size = int(input("n-gram size: "))
    genome_lst = proccess_data(fasta_file)
    obj_list = genome_nrgam(genome_lst, n_size)
    tree_list = []
    for i in range(len(obj_list)):
        tree_list.append(Tree(obj_list[i]))
    genome_list = create_tree(tree_list, n_size)
    print(genome_list[0])
    return genome_lst
main()