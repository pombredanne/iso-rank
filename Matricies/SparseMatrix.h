//
//  SparseSparseMatrix.h
//  Graph Matching
//
//  Created by Ali Hajimirza on 7/9/13.
//  Copyright (c) 2013 Ali Hajimirza. All rights reserved.
//

#ifndef _SparseMatrix_h
#define _SparseMatrix_h

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <unordered_map>
#include "SparseElement.h"
#include "Point.h"
#include "MatrixExceptions.h"

#ifdef __linux__
#include "dsmatrxa.h"
#include "ardsmat.h"
#include "ardssym.h"
#include "lsymsol.h"
#endif

template <typename T>
class SparseMatrix;

template <typename T>
std::ostream& operator<< (std::ostream&, const SparseMatrix<T>&);

template <typename T>
class SparseMatrix
{
private:
    const static int _DEFAULT_MATRIX_SIZE = 1;
    const static T _DEFAULT_MATRIX_ENTRY = 1;
    size_t _getArrSize() const;
    void _copy(const SparseMatrix<T>&);
    bool _hasEdge(const SparseElement<T>&);
    bool _hasEdge(const Point&);
    void _sort();
    
protected:
    int _rows;
    int _cols;
    std::unordered_map<Point,T> _edge_map;
    std::vector<SparseElement<T> > _edges;
   
public:
    /**************
     *Constructors*
     **************/
    SparseMatrix();
    SparseMatrix(int, int);
    SparseMatrix(const std::string&);
    SparseMatrix(const SparseMatrix<T>&);
    
    /************
     *Destructor*
     ************/
    virtual ~SparseMatrix();
    
    /***********
     *ACCESSORS*
     ***********/
    bool isSquare() const;
    bool isSymmetric() const;
    int getNumberOfRows();
    int getSparseFormSize();
    int getNumberOfColumns();
    std::vector<T> getSumOfRows();
    std::vector<T> getTopEigenVector();
    std::vector<int> getNeighbors(int vertex);
    //sparse_SparseMatrix_element<T>** getSparseForm();
    SparseMatrix<T> getScatteredSelection(const std::vector<int>& vec_A, const std::vector<int> vec_B);
    
    /**********
     *MUTATORS*
     **********/
    void insert(int, int, T);
    
    /**********
     *OPERATORS*
     **********/
    T operator()(int,int) const;
    void operator()(int,int,T);
    SparseMatrix<T> kron(const SparseMatrix<T>&); /* WORKS SHOULD BE CHANGED */
    bool operator==(const SparseMatrix<T>&);
    SparseMatrix<T>& operator= (const SparseMatrix<T>&);
    SparseMatrix<T> diagonalVectorTimesSparseMatrix(const std::vector<T>&);
    SparseMatrix<T> SparseMatrixTimesDiagonalVector(const std::vector<T>&);
    friend std::ostream& operator<< <> (std::ostream& stream, const SparseMatrix<T>& SparseMatrix);

    /* WILL IMPLEMENT IF I HAD TIME
     SparseMatrix<T> operator* (const SparseMatrix<T>&);
     SparseMatrix<T> operator+ (const SparseMatrix<T>&);
     SparseMatrix<T> operator- (const SparseMatrix<T>&);
     SparseMatrix<T> operator* (T);
     SparseMatrix<T> operator+ (T);
     SparseMatrix<T> operator- (T);
     void operator*= (const SparseMatrix<T>&);
     void operator+= (const SparseMatrix<T>&);
     void operator-= (const SparseMatrix<T>&);
     void operator*= (T);
     void operator+= (T);
     void operator-= (T);
     */
    
};

//==========================================================CONSTRUCTORS============================================================

template <typename T>
inline SparseMatrix<T>::SparseMatrix()
{
    this->_rows = _DEFAULT_MATRIX_SIZE;
    this->_cols = _DEFAULT_MATRIX_SIZE;
}

template<typename T>
inline SparseMatrix<T>::SparseMatrix(const std::string& file_path)
{
    int tmp_x;
    int tmp_y;
    std::ifstream file_reader;
    file_reader.open(file_path.c_str());
    
    if(file_reader.fail())
    {
        file_reader.close();
        throw FileDoesNotExistException(file_path.c_str());
    }
    
    file_reader >> this->_rows;
    file_reader >> this->_cols;
    file_reader >> tmp_x;      //skip the line number
    
    while (!file_reader.eof())
    {
        file_reader >> tmp_x;
        file_reader >> tmp_y;
        //making the matrix 0 indexed
        tmp_x--;
        tmp_y--;
        this->_edges.push_back(SparseElement<T>(tmp_x, tmp_y, _DEFAULT_MATRIX_ENTRY));
        this->_edge_map[Point(tmp_x, tmp_y)] = _DEFAULT_MATRIX_ENTRY;
    }
    std::sort(this->_edges.begin(), this->_edges.end());
    file_reader.close();
}

template <typename T>
inline SparseMatrix<T>::SparseMatrix(int rows, int cols)
{
    this->_rows = rows;
    this->_cols = cols;
}

template <typename T>
inline SparseMatrix<T>::SparseMatrix(const SparseMatrix<T>& SparseMatrix)
{
    _copy(SparseMatrix);
}

//==========================================================DESTRUCTOR==============================================================
template <typename T>
inline SparseMatrix<T>::~SparseMatrix()
{
}

//===========================================================ACCESSORS===============================================================
template <typename T>
inline int SparseMatrix<T>::getNumberOfRows()
{
    return this->_rows;
}

template <typename T>
inline int SparseMatrix<T>::getNumberOfColumns()
{
    return this->_cols;
}

template <typename T>
inline bool SparseMatrix<T>::isSquare() const
{
    return (this->_rows == this->_cols);
}

template <typename T>
inline bool SparseMatrix<T>::isSymmetric() const
{
    //sym. SparseMatrix has to be square
    if (this->_rows != this->_cols)
    {
        return false;
    }
    
    //chaking for entries to be equal
    for(int i = 0; i < this->_rows; i++)
    {
        for(int j = 0; j < this->_cols; j++)
        {
            if (this->_edges[(i * this->_cols) + j] != this->_edges[(j * this->_cols) + i])
            {
                return false;
            }
        }
    }
    
    return true;
}

//template <typename T>
//sparse_SparseMatrix_element<T>** SparseMatrix<T>::getSparseForm(){
//    
//    if(sparse_form != NULL)
//    {
//        return this->sparse_form;
//    }
//    
//    for(int i = 0; i < this->_getArrSize(); i++)
//    {
//        if(this->_edges[i] != 0)
//        {
//            sparse_form_size++;
//        }
//    }
//    
//    sparse_form = new sparse_SparseMatrix_element<T>* [sparse_form_size];
//    int counter=0;
//    
//    for(int i = 0; i < this->_getArrSize(); i++)
//    {
//        if(this->_edges[i] != 0)
//        {
//            sparse_SparseMatrix_element<T> *ele= new sparse_SparseMatrix_element<T>;
//            ele->row_index = i / this->_rows;
//            ele->col_index = i % this->_rows;
//            ele->value = this->_edges[i];
//            sparse_form[counter]=ele;
//            counter++;
//        }
//    }
//    
//    return this->sparse_form;
//}

template <typename T>
inline SparseMatrix<T> SparseMatrix<T>::getScatteredSelection(const std::vector<int>& vec_A, const std::vector<int> vec_B)
{
    int num_in_A = 0;
    for (int i=0; i< vec_A.size(); i++)
    {
        if (vec_A[i] == 1)
        {
            num_in_A++;
        }
    }
    int num_in_B = 0;
    for (int i=0; i < vec_B.size(); i++)
    {
        if( vec_B[i] == 1)
        {
            num_in_B++;
        }
    }
    //Initializing and allocating the product SparseMatrix
    SparseMatrix<T> res_SparseMatrix(num_in_A, num_in_B);
    
    int counter = 0;
    
    for (int i=0; i < vec_A.size(); i++)
    {
        for(int j=0; j < vec_B.size(); j++)
        {
            if ( vec_A[i] == 1 && vec_B[j] ==1)
            {
                res_SparseMatrix._edges[counter] = (*this)(i,j);
                counter++;
            }
        }
    }
    return res_SparseMatrix;
}



template <typename T>
inline std::vector<int> SparseMatrix<T>::getNeighbors(int vertex)
{
    std::vector<int> neighbors;
    
    for(int i = 0; i < this->getNumberOfRows(); i++)
    {
        if(this->_edges[(i * this->_cols) + vertex] == 1)
        {
            neighbors.push_back(i);
        }
    }
    
    return neighbors;
}

template <typename T>
inline std::vector<T> SparseMatrix<T>::getTopEigenVector(){
    
    //    int arr_size= (0.5 * this->_rows * (this->_rows+1));
    //    double nzval[arr_size];
    //    int counter = 0;
    //
    //    for(int i = 0; i < this->_rows; i++)
    //    {
    //        for(int j = i; j < this->_rows; j++)
    //        {
    //            nzval[counter] = (*this)(i,j);
    //            counter++;
    //        }
    //    }
    //
    //    ARdsSymSparseMatrix<T> ARSparseMatrix(this->_rows,nzval,'L');
    //    ARluSymStdEig<T> eigProb(1, ARSparseMatrix, "LM", 10);
    //    eigProb.FindEigenvectors();
    //    std::vector<T>* eigenVec = new std::vector<T>(eigProb.GetN());
    //
    //    for (int i=0; i < eigProb.GetN() ; i++)
    //    {
    //        eigenVec[i] = eigProb.Eigenvector(0,i);
    //    }
    //
    //    return eigenVec;
}


/*
 * returns a vector of type T where each entry corresponds to sum of entries in a SparseMatrix row.
 * Delete this object after usage.
 * @return std::vector<T>
 */
template <typename T>
inline std::vector<T> SparseMatrix<T>::getSumOfRows()
{
    std::vector<T>* sum_vector= new std::vector<T>(this->_rows);
    for(int i = 0; i < this->_getArrSize(); i++)
    {
        (*sum_vector)[(i / this->_rows)] += this->_edges[i];
    }
    
    return sum_vector;
}

//===========================================================MUTATORS================================================================



template <typename T>
inline void SparseMatrix<T>::insert(int i, int j, T value)
{
    if (value == 0)
        return;
    if ( (i >= _rows) || (i < 0) || (j >= _cols) || (j < 0) )
    {
        throw IndexOutOfBoundsException();
    }
    
    this->_edges.push_back(SparseElement<T>(i, j, value));        //has to change
    std::sort (this->_edges.begin(), this->_edges.end());          //tmp fix
}

//==========================================================OPERATORS================================================================
template <typename T>
inline SparseMatrix<T> SparseMatrix<T>::kron(const SparseMatrix<T>& matrix)
{
    // checking for matrices to be square
    if (!this->isSquare() || !matrix.isSquare())
    {
       // throw NotASquareSparseMatrixException();
    }
    
    //Initializing and allocating the product SparseMatrix
    int prod_size = this->_rows * matrix._rows;
   // SparseMatrix<T>* prod_matrix_right = new SparseMatrix<T>(prod_size, prod_size);
    SparseMatrix<T> prod_matrix(prod_size, prod_size);
    prod_matrix._edges = std::vector<SparseElement<T> >(this->_getArrSize()*matrix._getArrSize());

    int counter = 0;
    for(int i = 0; i < this->_getArrSize(); i++)
    {
        for(int j = 0; j < matrix._getArrSize(); j++)
        {
            prod_matrix._edges[counter++] = (SparseElement<T>((this->_edges[i].getX()*matrix._rows) + matrix._edges[j].getX(),
                                                              (this->_edges[i].getY()*matrix._cols) + matrix._edges[j].getY(),
                                                              this->_edges[i].getValue() *  matrix._edges[j].getValue()));
        }
    }
    
    std::sort(prod_matrix._edges.begin(), prod_matrix._edges.end());
    return prod_matrix;
}

template <typename T>
inline SparseMatrix<T> SparseMatrix<T>::diagonalVectorTimesSparseMatrix(const std::vector<T>& vec)
{
    if(this->_rows != vec.size())
    {
        throw DimensionMismatchException();
    }
    
    SparseMatrix<T> ret_SparseMatrix (*this);
    for(int i = 0; i < this->_getArrSize(); i++)
    {
        ret_SparseMatrix._edges[i] = vec[i/this->_rows] * this->_edges[i];
    }
    
    return ret_SparseMatrix;
}

template <typename T>
inline SparseMatrix<T> SparseMatrix<T>::SparseMatrixTimesDiagonalVector(const std::vector<T>& vec)
{
    if(this->_cols != vec.size())
    {
        throw DimensionMismatchException();
    }
    
    SparseMatrix<T> ret_SparseMatrix(*this);
    for(int i = 0; i < this->_getArrSize(); )
    {
        for(int j = 0; j < vec.size(); j++)
        {
            ret_SparseMatrix._edges[i] = this->_edges[i] * vec[j];
            i++;
        }
    }
    
    return ret_SparseMatrix;
}
/////////////////////////////////this point
template <typename T>
inline std::ostream& operator<<(std::ostream& stream, const SparseMatrix<T>& matrix)
{
    stream<< "Size: " << matrix._rows << "*" << matrix._cols << '\n';
    for (int i = 0; i < matrix._rows; i++)
    {
        for (int j = 0; j < matrix._cols; j++)
        {
            stream << matrix(i, j) << ' ';
        }
        stream << "\n";
    }
    stream << "\n\n\n";
    return stream;
}

template <typename T>
inline T SparseMatrix<T>::operator()(int i, int j) const
{
    if ( (i >= _rows) || (i < 0) || (j >= _cols) || (j < 0) )
    {
        throw IndexOutOfBoundsException();
    }
    
    Point p(i,j);
    std::unordered_map<Point,int>::const_iterator find_res = this->_edge_map.find(p);
    
    if ( find_res == this->_edge_map.end() )
        return 0;
    else
        return find_res->second;
}

template <typename T>
inline void SparseMatrix<T>::operator()(int i, int j, T value)
{
    if (value == 0)
        return;
    
    if ( (i >= _rows) || (i < 0) || (j >= _cols) || (j < 0) )
    {
        throw IndexOutOfBoundsException();
    }
    
    this->_edges_map[Point(i,j)] = value;
    
    this->_edges.push_back(SparseElement<T>(i, j, value));        //has to change
    std::sort (this->_edges.begin(), this->_edges.end());
}

template <typename T>
inline SparseMatrix<T>& SparseMatrix<T>::operator=(const SparseMatrix<T>& rhs)
{
    _copy(rhs);
}

template <typename T>
inline bool SparseMatrix<T>::operator==(const SparseMatrix<T>& rhs)
{
    // checking for dimension equality
    if ((this->_cols != rhs._cols) || (this->_rows != rhs._rows) || (this->_getArrSize() != rhs._getArrSize()))
    {
        return false;
    }
    
    //checking for entry equality
    for (int i = 0; i < this->_getArrSize(); i++)
    {
        for( int j = 0; j< this->_cols; j++)
        {
            if ((*this)(i,j) != rhs(i,j))
                return false;
        }
    }
    
    return true;
}

//===========================================================PRIVATE=================================================================
template <typename T>
inline void SparseMatrix<T>::_copy(const SparseMatrix<T>& rhs)
{
    this->_rows = rhs._rows;
    this->_cols = rhs._cols;
    
    for(int i = 0; i < this->_getArrSize(); i++)
    {
        this->_edges[i] = rhs._edges[i];
    }
    
    this->_edge_map = std::unordered_map<Point, T>(rhs._edges_map);
}

template<typename T>
inline size_t SparseMatrix<T>::_getArrSize() const
{
    return this->_edges.size();
}
    
template <typename T>
inline bool SparseMatrix<T>::_hasEdge(const SparseElement<T> & edge)
{
    return std::binary_search(this->_edges.begin(), this->_edges.end(), edge);
}
    
template <typename T>
inline bool SparseMatrix<T>::_hasEdge(const Point& point)
{
    return (this->_edges_map.find(point) == this->_edges_map.end());
}

template <typename T>
inline void SparseMatrix<T>::_sort()
{
    std::sort(this->_edges.begin(), this->_edges.end());
}


//===================================================================================================================================
    

#endif
