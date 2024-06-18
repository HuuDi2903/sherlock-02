/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////


// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Chracter;
class Criminal;
class Sherlock;
class Watson;
class Robot;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyInPinkProgram;

class BaseItem;
class MagicBook;
class EnergyDrink;
class FirstAid;
class ExcemptionCard;
class PassingCard;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C = 0, S, W, SW };


class MapElement {
friend class TestStudyInPink;
protected:
    ElementType type;
public:
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement {
    public:
        Path();
};

class Wall : public MapElement {
    public:
        Wall();
};

class FakeWall : public MapElement {
    private:
        int req_exp, r, c;
    public:
        FakeWall (int in_req_exp);
        int getReqExp() const;
};


class Map {
friend class TestStudyInPink;
private:
    int num_rows, num_cols;
    int num_walls;
    Position * array_walls;
    int num_fake_walls;
    Position * array_fake_walls;
    MapElement*** map;
public:
    Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls);
    ~Map();
    bool isValid( const Position & pos , MovingObject * mv_obj ) const ;
};
 

class Position {
friend class TestStudyInPink;
private:
    int r, c;
public:
    static const Position npos;

    Position(int r = 0, int c = 0);

    Position(const string & str_pos);

    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);

    string str() const;

    bool isEqual (int in_r , int in_c ) const {
        return (in_r == this->r && in_c == this->c);
    }

    bool isEqual(const Position & pos) const;    
};


class MovingObject {
friend class TestStudyInPink;
protected:
    int index;
    Position pos;
    Map * map;
    string name;
    const Position init_pos;
    int exp, hp;
public:
    MovingObject(int index, const Position pos, Map * map, const string & name);
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    virtual Position getCurrentPosition() const = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    string getName() const;
    int getEXP() const;
    int getHP() const;
    void setEXP(int exp);
    void setHP(int hp);
};


class Character : public MovingObject {
friend class TestStudyInPink;
protected:
    string moving_rule;
    BaseBag *bag;  // the kind of bag of the character
    int currentStep = 0;
public:
    Character(int index, const Position &init_pos, Map *map, const string & name);
    virtual Position getNextPosition();
    virtual Position getCurrentPosition() const;
    virtual void move();
    virtual string str() const = 0;
    BaseBag *getBag () const;
};


class Sherlock : public Character /* TODO */ {
public:
    Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    ~Sherlock() {};
    // move
    void move();
    // str
    string str() const;
    // Case of Sherlock meeting Watson
    bool meetWatson (Watson *watson);
    // Cases of Sherlock meeting a Robot
    bool meetRobot (Robot *robot);
    // Case of Sherlock meeting RobotC
    bool meetRobotC (Criminal *criminal, Robot *robot);
};


class Watson : public Character /* TODO */ {
public:
    Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    ~Watson() {};
    // move
    void move();
    // str
    string str() const;
    // Case of Watson meeting Sherlock
    bool meetSherlock (Sherlock *sherlock);
    // Cases of Watson meeting a Robot
    bool meetRobot (Robot *robot);
    // Case of Watson meeting RobotC
    bool meetRobotC (Criminal *criminal, Robot *robot);
};


class Criminal : public Character /* TODO */ {
private:
    Sherlock *sherlock;
    Watson *watson;
    Robot *robot;
    int criminalStep;
    Position prev_pos;
    int check = 0;
public:
    Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson);
    ~Criminal() {};
    // getNextPosition
    Position getNextPosition();
    // move
    void move();
    // str
    string str() const;
    // ...
    Robot *getRobot() const {
        return robot;
    }
    int getcriminalStep() const {
        return criminalStep;
    };
    Position getPreviousPosition();
};


class ArrayMovingObject {
friend class TestStudyInPink;
private:
    int count;  // the current number of elements in the array
    int capacity;  // maximum number of elements in the array
    MovingObject** arr_mv_objs;
public:
    ArrayMovingObject(int capacity);
    ~ArrayMovingObject() ;
    bool isFull() const;
    bool add(MovingObject * mv_obj);
    MovingObject * get(int index) const;
    int size() const;  // return the current number of elements in the array
    string str() const;
};


class Configuration {
friend class StudyInPinkProgram;
friend class TestStudyInPink;
private:
    // TODO
    int map_num_rows;            
    int map_num_cols;        
    int max_num_moving_objects;  
    int num_walls;               
    Position* arr_walls;           
    int num_fake_walls;       
    Position* arr_fake_walls;      
    string sherlock_moving_rule; 
    Position sherlock_init_pos;   
    int sherlock_init_hp, sherlock_init_exp, watson_init_hp, watson_init_exp;   
    string watson_moving_rule;
    Position watson_init_pos;
    Position criminal_init_pos; //! vị trí của tên trộm [x,y]
    int num_steps;
public:
    Configuration(const string & filepath);
    ~Configuration();
    string str() const;
};


// Robot, BaseItem, BaseBag,...

class Robot : public MovingObject {
protected:
    RobotType robot_type;
    BaseItem *item;
    Criminal *criminal;
public:
    Robot (int index, const Position &init_pos, Map *map, RobotType robot_type);
    static int countRobot;
    static Robot *createRobot(int index, Map *map, Sherlock *sherlock, Watson *watson, Criminal *criminal);
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const;
    void move();
    virtual BaseItem *getItem();
    int getRobotType() const;
    int getRow() const;
    int getCol() const;
};


class RobotC : public Robot {
private:
    Criminal* criminal;
public:
    RobotC ( int index , const Position & init_pos , Map * map , Criminal * criminal ) ;
    Position getNextPosition();
    int getDistance(Character * character) const;
    string str() const;
};


class RobotS : public Robot {
private:
    Sherlock* sherlock;
    Criminal* criminal;
public:
    RobotS (int index , const Position & init_pos , Map * map , Criminal *
criminal , Sherlock * sherlock) ;
    Position getNextPosition();
    int getDistance() const;
    string str() const;
};


class RobotW : public Robot {
private:
    Watson* watson;
    Criminal* criminal;
public:
    RobotW ( int index , const Position & init_pos , Map * map , Criminal * criminal , Watson * watson) ;
    Position getNextPosition();
    int getDistance() const;
    string str() const;
};


class RobotSW : public Robot {
private:
    Sherlock* sherlock;
    Watson* watson;
    Criminal* criminal;
public:
    RobotSW ( int index , const Position & init_pos , Map * map , Criminal * criminal , Sherlock * sherlock , Watson * watson) ;
    Position getNextPosition();
    int getDistance() const;
    string str() const;
};


class BaseItem {
friend class TestStudyInPink;
public:
    BaseItem() {};
    virtual bool canUse ( Character * obj , Robot * robot ) = 0;
    virtual void use ( Character * obj , Robot * robot ) = 0;
    virtual ItemType getItemType() const = 0;
};


class MagicBook : public BaseItem {
public:
    bool canUse (Character *obj, Robot *robot);
    void use (Character *obj, Robot *robot);
    ItemType getItemType() const;
};


class EnergyDrink : public BaseItem {
public:
    bool canUse (Character *obj, Robot *robot);
    void use (Character *obj, Robot *robot);
    ItemType getItemType() const;
};


class FirstAid : public BaseItem {
public:
    bool canUse (Character *obj, Robot *robot);
    void use (Character *obj, Robot *robot);
    ItemType getItemType() const;
};


class ExcemptionCard : public BaseItem {
public:
    bool canUse (Character *obj, Robot *robot);
    void use (Character *obj, Robot *robot);
    ItemType getItemType() const;
};


class PassingCard : public BaseItem {
private:
    string challenge;
public:
    PassingCard (const string &challenge);
    bool canUse (Character *obj, Robot *robot);
    void use (Character *obj, Robot *robot);
    ItemType getItemType() const;
};


class BaseBag {
protected: 
    class Node {
    public:
        BaseItem *item; 
        Node *next; 
        friend class BaseBag;
    public:
        Node(BaseItem *item, Node *next = nullptr) : item(item), next(next) {};
    };
protected:
    Character * obj;
    int size; 
    int capacity;
    Node* head;
public:
    BaseBag(int capacity) : capacity(capacity), size(0), head(nullptr) {};
    ~BaseBag() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp->item;
            delete temp;
        }
    };
    virtual bool insert ( BaseItem * item ) ;  // return true if insert successfully
    virtual BaseItem * get () ;  // return the item as described above , if not found , return NULL
    virtual BaseItem * get ( ItemType itemType ) ;  // return the item as described above , if not found , return NULL
    virtual string str () const ;
    int getSize() const {
        return size;
    } ;
};


class SherlockBag : public BaseBag {
private:
    Sherlock *sherlock;
public:
    SherlockBag (Sherlock *sherlock);
    bool canUse (Character *obj, Robot *robot);
    void use (Character *obj, Robot *robot);
    ItemType getItemType() const;
};


class WatsonBag : public BaseBag {
private:
    Watson *watson;
public:
    WatsonBag (Watson *watson);
    bool canUse (Character *obj, Robot *robot);
    void use (Character *obj, Robot *robot);
    ItemType getItemType() const;
};


class StudyInPinkProgram {
friend class TestStudyInPink;
private:
    // Sample attributes    
    Configuration * config;
    
    Sherlock * sherlock;
    Watson * watson;
    Criminal * criminal;
    
    Map * map;
    ArrayMovingObject * arr_mv_objs;


public:
    StudyInPinkProgram(const string & config_file_path);

    bool isStop() const;

    void printResult() const;

    void printStep(int si) const;

    void run(bool verbose);

    ~StudyInPinkProgram() {
    delete map;
    delete arr_mv_objs;
    delete config->arr_walls;
    delete config->arr_fake_walls;
    delete sherlock;
    delete watson;
    delete criminal;
    };
};


////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
