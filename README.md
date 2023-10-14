# WordleSolver

### Credit:
Credit for the dictionary goes to `mchirico`, whose [file](https://sourceforge.net/projects/souptonuts/files/latest/download) I used ([linux.words](OldDictionaryFiles/linux.words]), a simple python [script](OldDictionaryFiles/MakeDictionary.py), to make the separate dictionary files for 5-12 letter words.

### Description:
Gives user all possible solutions to wordle puzzles based on in-game feedback (user provides to program)

### Installation/Execution:
Requires c++ compiler, recommend using g++. Python files are not necessary. Recommend running from root of this directory as the dictionaries need to be in the `Dictionaries` folder in the root directory.

### User Interface:
Upon launching program, enter the number of letters you're playing with. Currently supported options are 5 through 12 letters. Use [this script](OldDictionaryFiles/MakeDictionary.py) to make new dictionaries based off of the [linux.words](OldDictionaryFiles/linux.words) file.

The main menu will look like this:
```?
WORDLE SOLVER:

Enter a search string or one of the options below:
        P : prints current searchlist (everything that meets criteria)
        H : display help for using search strings
        S : display summary of searchlist (number items)
        N : change number of letters for this game
        C : resets current search criteria
        Q : quits this application

>> 
```
This menu will accept multiple inputs. The single-character options listed are not case-sensitive. Enter a search string that matches the criteria below (also accessed by entering `h` or `H` in the main menu.

```?
HELP WITH CREATING SEARCH STRINGS:

	__s_D   :  lowercase letters are correct but wrong place, uppercase are correct and in correct place
	.itna   :  a period before letters will exclude words containing those letters
	#posed  :  guess if a word meets current criteria (displays success/failure)
```
**IMPORTANT:** 
* Use underscores `_` for blank spaces
* Using search strings **is** case-sensitive to deliniate correct vs incorrect placement:
	* `stR__` will include an **R** in position 3, and include the letters **s** and **t** but not in positions 1 and 2, respectively
 	* `___iP` includes **P** in position 5, and includes **i** in any position but 4 (and 5 by nature of the **P**)
	* `S___p` keeps the **S** in position 1, and includes a **p** in any position but 1 and 5 
* Users cannot combine types of search strings but must enter them separately:
	* `stR.a.p`
 	* `__s_D#strum`
  	* `STRU.t`
  	* These are all invalid search strings

### Example Use Cases (With Pictures):
[Example 1](Documentation/Example1.md)

[Example 2](Documentation/Example2.md)

[Example 3](Documentation/Example3.md)

### Disclaimer:
Words are nearly always solved by the 5th guess, but please do not hold me liable if this does not solve your wordle appropriately. I have found several words not in the master dictionary that were my puzzle (i.e. fetus). Be a human being and be smarter than the puzzle, and please push a change to my code so we can make the dictionary better for everyone.
