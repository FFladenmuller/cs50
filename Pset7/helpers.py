from enum import Enum


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())


def getMin(matrix, i, j, a, b):

    # Calculate cost for each possible choice
    deletion = matrix[i - 1][j][0] + 1
    insertion = matrix[i][j - 1][0] + 1
    substitution = matrix[i - 1][j - 1][0] + 1 if a[i - 1] != b[j - 1] else matrix[i - 1][j - 1][0]
    choices = (deletion, insertion, substitution)

    # Choose min cost
    cost = min(choices)

    # Figure out which operation was used
    if cost == deletion:
        operation = Operation.DELETED
    elif cost == insertion:
        operation = Operation.INSERTED
    else:
        operation = Operation.SUBSTITUTED
    choice = (cost, operation)
    return choice


def distances(a, b):
    """Calculate edit distance from a to b"""

    # Create empty matrix size of [len(a) + 1 rows][len(b) + 1 columns]
    #                                       len(b) columns                 len(a) rows
    # [y][x]
    matrix = [[[None for i in range(2)] for x in range(len(b) + 1)] for y in range(len(a) + 1)]
    # Initialize top row of matrix
    for i in range(1, len(b) + 1, 1):
        matrix[0][i] = (i, Operation.INSERTED)
    # Initialize leftmost column of matrix
    for j in range(1, len(a) + 1, 1):
        matrix[j][0] = (j, Operation.DELETED)
    # Initialize 0,0 to 0, None as no work has been done.
    matrix[0][0] = (0, None)

    ''' Iterate through cells in matrix, filling in values by finding the minimum cost for each cell.
        We can find this by choosing the operation that will give us the least cost to convert the i
        characters of A to the j characters of B. There are 3 operations we can use.
        1) Deletion - convert i - 1 letters to j characters and delete the last character
        2) Insertion - convert i characters to j - 1 characters and insert last character
        3) Substitution convert i - 1 characters to j - 1 characters and substitute last letter
    '''
    for i in range(1, len(a) + 1, 1):
        for j in range(1, len(b) + 1, 1):
            matrix[i][j] = getMin(matrix, i, j, a, b)
    return matrix
