Our algorithm has been tested for correctness on the Windows operating system with C++11.

ERRATA:

During the conception and integration of our algorithm into a wholesome model, we encountered the following inconsistencies in the files provided to us: 

* The inclusion of proper nouns in the provided files was not in line with the sample test cases. To further elaborate, a few proper nouns were mentioned in the test cases, (for example: London, New York) but not inlcuded in the relevant placeholder files. 

* Multiple test cases included the mention of "London" as a place, whereas the same was not included in "places.txt" as a valid place input. To come around this issue, we have included London as a valid place in the placeholder data structure within our code.

* "Update", naturally exists as a "tell" concept, but was not included in the text file "tell_concept.txt". To overcome this problem, we could include "Update" into our concepts data structure, which we have not done so as to minimize hard-coding. Since we were not allowed to modify the files provided to us and were supposed to keep the code as generalized as possible, we have left Case #7 with an incomplete output. To generate the correct output, the file can be modified to include the valid string or "Update" can be hard-coded into the data structure.

* "Amitabh" is the name of a person and hence a proper noun. Naturally, we expected the name to be included in the "contact_name.txt" file. Our generalized algorithm could comfortably handle the given Case, but the text file must be updated to include "Amitabh" and any other desired names to create a more confident output.

ABSTRACT:

Our algorithm uses an approach in which we assign scalar scores(representative of the probability that the corresponding grammar is the grammar of the input string) to each of the grammars based on the concepts/placeholder/keywords present in the input string and finally the grammar with the highest scalar score is written to the final output file.

DATA STRUCTURES USED: 

(These datastructures are generated from the provided text files)

1. concepts: It is a dictionary (map) whose keys are the concept names and value is a vector which constitutes of all the possible strings that make up the concept. (This is provided. The concepts present in this datastructure are extracted from all the text files present in the 'Concepts' directory)
2. placeholder: It is a dictionary (map) whose keys are the placeholder names and value is a vector which constitutes of all the possible strings that make up the placeholder. (This is provided. The placeholders present in this datastructure are extracted from all the text files present in the 'Placeholders' directory)
3. grammar: It is a dictionary of grammar name and its values contain the corresponding concepts, placeholders and keywords.
4. counters: It is a dictionary (map) whose keys are the provided grammar names and its values are scalar scores representative of the probability that the corresponding grammar is the grammar of the input string. Thus the grammar with the highest scalar score is the required grammar.

CUSTOM RESOURCE FILES:

1. reject_words.txt: It contains a collection of all common words that are not relevant enough to be used as keywords that might increase the probability of a certain grammar being related to a given input string. For example: a, an, the, for, etc.

2. conjunctions.txt: It contains a collection of all common conjunctions that might have been used in the input string to separate two different commands present in an input string. For example: and, meanwhile, also, etc.

ALGORITHM:

1. Read the input string
2. Check whether there are two commands present within the same input string (with the help of the entries in conjunctions.txt)
3. Make a vector of all the concepts which are present in the input string.
4. Make a vector of all the placeholders which are present in the input string.
5. Increase the scalar score of all the grammars which contain the concepts found in the input string.
6. Increase the scalar score of all the grammars which contain the placeholders found in the input string.
7. Increase the scalar score of all the grammars which contain the relevant keywords found in the input string (depending on the relevance of keywords, some of them might have a greater incrementation effect on the grammar).
8. Thus the grammar with the highest scalar score (counter score) is the required grammar.

DESCRIPTION OF FUNCTIONS:

1. make_placeholder, make_grammar, make_concept, make_custom_resource, make_conjunctions: These functions populate all the described datastructures by reading the provided text files.
2. preKMP, KMP: These functions implement the Knuth-Morris-Pratt string matching algorithm (this is one of the fastest string matching algorithms).
3. is_concept_present, concepts_that_are_present: These functions generate the vector of concepts which are present in the input string.
4. is_placeholder_present, placeholders_that_are_present: These functions generate the vector of placeholders which are present in the input string.
5. find_grammar_scores_concept, find_grammar_scores_placeholder, find_grammar_scores_keywords: These functions provide each of the grammars their corresponding scalar scores based on the presence of the concepts, placeholders and keywords in the input string.
6. split: In case of the presence of two commands in the same input string, this function splits the input string into two independent parts containing each of the two different commands, and returns them as a pair.
7. command, give_command: These functions work in unison to return the grammar that the input string corresponds to.