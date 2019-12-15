import math
from math import sqrt
import numbers

def zeroes(height, width):
        """
        Creates a matrix of zeroes.
        """
        g = [[0.0 for _ in range(width)] for __ in range(height)]
        return Matrix(g)

def identity(n):
        """
        Creates a n x n identity matrix.
        """
        I = zeroes(n, n)
        for i in range(n):
            I.g[i][i] = 1.0
        return I
    

class Matrix(object):

    # Constructor
    def __init__(self, grid):
        self.g = grid
        self.h = len(grid)
        self.w = len(grid[0])
    
    # Primary matrix math methods
    #############################
    
    def mat_vector_mul(self,other_T):
        '''
        create a matrix vector multiplication with other matrix vector in row wise
        '''
        mat_mul_2list = []
        row_data = []
        total = 0
        
        for i in range(self.h):
            for k in range(other_T.h):
                for j in range(other_T.w):   
                    total = total + (self.g[i][j] * other_T.g[k][j])
                row_data.append(total)
                total = 0
            mat_mul_2list.append(row_data)
            row_data = []
            mult_matrix = Matrix(mat_mul_2list)  

        return mult_matrix
 
    def determinant(self):
        """
        Calculates the determinant of a 1x1 or 2x2 matrix.
        """
        if not self.is_square():
            raise Exception(ValueError, "Cannot calculate determinant of non-square matrix.")
        if self.h > 2:
            raise Exception(NotImplementedError, "Calculating determinant not implemented for matrices largerer than 2x2.")
        
        # TODO - your code here
        if self.h == 2:
            return (self.g[0][0]*self.g[1][1]) - (self.g[1][0]*self.g[0][1]) 

    def trace(self):
        """
        Calculates the trace of a matrix (sum of diagonal entries).
        """
        if not self.is_square():
            raise Exception(ValueError, "Cannot calculate the trace of a non-square matrix.")
    
        dig_sum = 0 
        for i in range(self.h):
            for j in range(self.w):
                if i == j:
                    dig_sum = dig_sum + self.g[i][j]
        return dig_sum

    def inverse(self):
        """
        Calculates the inverse of a 1x1 or 2x2 Matrix.
        """
        inverse_2list = []
        row_data = []
        row_data1 = []
        row_data2 = []
        
        if not self.is_square():
            raise(ValueError, "Non-square Matrix does not have an inverse.")
        if self.h > 2:
            raise(NotImplementedError, "inversion not implemented for matrices larger than 2x2.")
            
        if self.h == 1:
            row_data.append(1/self.g[0][0])
            inverse_2list.append(row_data)
            
        elif self.h == 2:
            det = self.determinant()
            if det == 0:
                raise "cannot devide by zero"
            else:
                row_data1 = [(1/det)*(self.g[1][1]),(-1/det)*(self.g[0][1])]
                row_data2 = [(-1/det)*(self.g[1][0]),(1/det)*(self.g[0][0])]
                inverse_2list.append(row_data1)
                inverse_2list.append(row_data2)
             
        inverse_matrix = Matrix(inverse_2list)

        return inverse_matrix
            
    def T(self):
        """
        Returns a transposed copy of this Matrix.
        """
        # TODO - your code here
        transpose_2list = []
        row_data = []
        for i in range(self.w):
            for j in range(self.h):
                row_data.append(self.g[j][i])
            transpose_2list.append(row_data)
            row_data = []
        transpose_matrix = Matrix(transpose_2list)    
        return transpose_matrix

    def is_square(self):
        return self.h == self.w

    
    # Begin Operator Overloading
    ############################
    def __getitem__(self,idx):
        """
        Defines the behavior of using square brackets [] on instances
        of this class.

        Example:

        > my_matrix = Matrix([ [1, 2], [3, 4] ])
        > my_matrix[0]
          [1, 2]

        > my_matrix[0][0]
          1
        """
        return self.g[idx]

    def __repr__(self):
        """
        Defines the behavior of calling print on an instance of this class.
        """
        s = ""
        for row in self.g:
            s += " ".join(["{} ".format(x) for x in row])
            s += "\n"
        return s

    def __add__(self,other):
        """
        Defines the behavior of the + operator
        """
        if self.h != other.h or self.w != other.w:
            raise(ValueError, "Matrices can only be added if the dimensions are the same") 
        row_data = []
        sum_2list = []
        for i in range(self.h):
            for j in range(self.w):
                row_data.append(self.g[i][j] + other.g[i][j])
            sum_2list.append(row_data)
            row_data = []
        
        sum_matrix = Matrix(sum_2list)
        return sum_matrix

    def __neg__(self):
        """
        Defines the behavior of - operator (NOT subtraction)

        Example:

        > my_matrix = Matrix([ [1, 2], [3, 4] ])
        > negative  = -my_matrix
        > print(negative)
          -1.0  -2.0
          -3.0  -4.0
        """
        neg_2list = []
        row_data = []
        for i in range(self.h):
            for j in range(self.w):
                row_data.append(-self.g[i][j])
            neg_2list.append(row_data)
            row_data = []
            
        negative_matrix = Matrix(neg_2list)
        return negative_matrix

    def __sub__(self, other):
        """
        Defines the behavior of - operator (as subtraction)
        """
        if self.h != other.h or self.w != other.w:
            raise(ValueError, "Matrices can only be added if the dimensions are the same") 
        row_data = []
        sub_2list = []
        for i in range(self.h):
            for j in range(self.w):
                row_data.append(self.g[i][j] - other.g[i][j])
            sub_2list.append(row_data)
            row_data = []
        sub_matrix = Matrix(sub_2list)    
        return sub_matrix

    def __mul__(self, other):
        """
        Defines the behavior of * operator (matrix multiplication)
        steps : 1) take transpose of other matrix
                2) calculate the vector mult of each row of self matrix with each row of other matrix
                3) return the matrix_multiplication
        """
        
        if self.w != other.h:
            return
        else:
            other_T = other.T()
            multiplication_matrix = self.mat_vector_mul(other_T)
            return multiplication_matrix
        
    def __rmul__(self, other):
        """
        Called when the thing on the left of the * is not a matrix.

        Example:

        > identity = Matrix([ [1,0], [0,1] ])
        > doubled  = 2 * identity
        > print(doubled)
          2.0  0.0
          0.0  2.0
        """
        multi_2list = []
        row_data = []
        if isinstance(other, numbers.Number):
            for i in range(self.h):
                for j in range(self.w):
                    row_data.append(self.g[i][j]*other)
                multi_2list.append(row_data)
                row_data = []
        multi_matrix = Matrix(multi_2list)
        return multi_matrix
            
            
            