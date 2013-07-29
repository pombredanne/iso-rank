/***********************************************************************************
 * Vertex Class. Used in Tarjan's Algorithm to keep track of which vertex belongs  *
 * to which component.                                                             *
 ***********************************************************************************/


#ifndef _vertex_h
#define _vertex_h

#include <iostream>

class vertex
{
    int vertex_name;
    long index;
    long low_link;
    int tarjan_flag;

 public:
    vertex();
    vertex (int, int);
    virtual ~vertex();
    vertex* operator=(const vertex *rhs);
    vertex& operator=(const vertex &rhs);
    bool operator!=(const vertex &rhs);
    bool operator==(const vertex &rhs);
    void set_index(long);
    void set_low_link(long);
    long get_index();
    long get_low_link();
    int get_vertex_name();
    int get_tarjan_flag();
};

/*vertex class default constructor
 */
vertex:: vertex(){
    this->vertex_name=-1;
    this->index=-1;
    this->low_link=-1;
    this->tarjan_flag=0;
}

/*vertex class constructor
 *@param: a is vertex_name
 *@param: b is the index used for Tarjan's Algorithm
 */
vertex::vertex(int a,int b){
    this->vertex_name=a;
    this->index=b;
    this->low_link=-1;
    this->tarjan_flag=0;
}

vertex::~vertex()
{
}

/*vertex class function
 *operator == that checks if this vertex is equal to parameter
 *@param:vertex to check for equality
 */
bool vertex::operator==(const vertex &rhs){
    return this->vertex_name==rhs.vertex_name;
}

/*vertex class function
 *operator != that checks if this vertex is not equal to parameter
 */
bool vertex::operator!=(const vertex &rhs){
    return !(this->vertex_name==rhs.vertex_name);
    
}

/*vertex class function
 *operator = sets this vertex to parameter
 *@param: vertex to set this vertex to
 */
vertex* vertex::operator=(const vertex *rhs){
    this->vertex_name=rhs->vertex_name;
    this->index=rhs->index;
    this->low_link=rhs->low_link;
    return this;
}

/*vertex class function
 *operator = sets this vertex to parameter
 */
vertex& vertex::operator=(const vertex &rhs){
    this->vertex_name=rhs.vertex_name;
    this->index=rhs.index;
    this->low_link=rhs.low_link;
    return *this;
}

/*vertex class function
 *sets this vertex's index to a
 *@param: integer to set index to
 */
void vertex::set_index(long a){
    this->index=a;
}

/*vertex class function
 *sets this vertex's low_link to b
 *@param: integer to set low link to
 */
void vertex::set_low_link(long b){
    //std::cout<< "setting low link of "<<this->get_vertex_name()<<  "to:" << b << std::endl;
    this->tarjan_flag=1;
    this->low_link=b;
}

/*vertex class function
 *returns this vertex's index
 */
long vertex:: get_index(){
    return this->index;
}

/*vertex class function
 *returns this vertex's low link
 */
long vertex:: get_low_link(){
    return this->low_link;
}

/*vertex class function
 *returns this vertex's name
 */
int vertex:: get_vertex_name(){
    return this->vertex_name;
}

/*vertex class function
 *returns this vertex's tarjan flag
 */
int vertex:: get_tarjan_flag(){
    return this->tarjan_flag;
}



#endif
