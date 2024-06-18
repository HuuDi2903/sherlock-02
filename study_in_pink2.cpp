#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////


//  TASK 3.1

// Constructor for class MapElement
MapElement::MapElement(ElementType in_type) {
    this->type = in_type;
}
MapElement::~MapElement() {};
ElementType MapElement::getType() const {
    return type;
}

// Constructor for class Path
Path::Path() : MapElement(PATH) {
    this->type = PATH;
}

// Constructor for class Wall
Wall::Wall() : MapElement(WALL) {
    this->type = WALL;
}

// Constructor for class FakeWall
FakeWall::FakeWall(int in_req_exp) : MapElement(FAKE_WALL) {
    this->type = FAKE_WALL;
    req_exp = in_req_exp;
}
int FakeWall::getReqExp() const {
    return req_exp;
}


//  TASK 3.2

// Constructor for class Map
Map::Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls) : num_rows(num_rows), num_cols(num_cols) {
    this->num_walls = num_walls;
    this->num_fake_walls = num_fake_walls;

    map = new MapElement **[num_rows];
    for (int i = 0; i < num_rows; i++) {
        map[i] = new MapElement *[num_cols];
        for (int j = 0; j < num_cols; j++) {
            map[i][j] = new Path();
        }
    }
    for (int i = 0; i < num_walls; i++) {
        map[array_walls[i].getRow()][array_walls[i].getCol()] = new Wall();
    }
    for (int i = 0; i < num_fake_walls; i++) {
        // Calculate in_rqp_exp of the fake wall
        map[array_fake_walls[i].getRow()][array_fake_walls[i].getCol()] = new FakeWall((array_fake_walls[i].getRow() * 257 + array_fake_walls[i].getCol() * 139 + 89) % 900 + 1);
    }
}
bool Map::isValid( const Position & pos , MovingObject * mv_obj ) const {
    if(pos.getRow() < 0 || pos.getRow() >= num_rows || pos.getCol() < 0 || pos.getCol() >= num_cols) {
        return false;
    }
    if (map[pos.getRow()][pos.getCol()]->getType() == WALL) {
        return false;
    }
    if (map[pos.getRow()][pos.getCol()]->getType() == FAKE_WALL) {
        if (mv_obj->getName() == "Sherlock" || mv_obj->getName() == "Criminal" || mv_obj->getName() == "RobotC" || mv_obj->getName() == "RobotS" || mv_obj->getName() == "RobotW" || mv_obj->getName() == "RobotSW") {
            return true;
        }
        else if (mv_obj->getName() == "Watson") {
            if (mv_obj->getEXP() >= ((FakeWall *)map[pos.getRow()][pos.getCol()])->getReqExp()) {
                return true;
            }
            return false;
        }
    }
    return true;
}
Map::~Map() {
    for(int i = 0; i < num_rows; i++){
        for(int j = 0; j < num_cols; j++){
            delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
}


//  TASK 3.3  POSITION

// Constructor for class Position
Position::Position(int r, int c) {
    this-> r = r;
    this-> c = c;
}
Position::Position(const string & str_pos) {
    sscanf(str_pos.c_str(), "(%d,%d)", &r, &c);
}
int Position::getRow() const {
    return r;
};
int Position::getCol() const {
    return c;
}
void Position::setRow(int r) {
    this-> r = r;
};
void Position::setCol(int c) {
    this-> c = c;
};
string Position::str() const {
    return "(" + to_string(r) + "," + to_string(c) + ")";
};
bool Position::isEqual(const Position & pos) const {
    return (r == pos.getRow() && c == pos.getCol());
};
const Position Position::npos(-1,-1);


//  TASK 3.4  MOVING OBJECT

// Constructor for class MovingObject
MovingObject::MovingObject(int index, const Position pos, Map * map, const string & name) {
    this ->index = index;
    this ->pos = pos;
    this ->map = map;
    this ->name = name;
}
MovingObject::~MovingObject() {};
Position MovingObject::getCurrentPosition() const {
    return pos;
}
string MovingObject::getName() const {
    return name;
}
int MovingObject::getEXP() const {
    return exp;
}
int MovingObject::getHP() const {
    return hp;
}
void MovingObject::setEXP(int exp) {
    if(exp > 900){
        this ->exp = 900;
    }
    else{
        this ->exp = exp; 
    }
}
void MovingObject::setHP(int hp) {
    if(hp > 500){
        this ->hp = 500;
    }
    else{
        this ->hp = hp;
    }
}


//  Class Character
Character::Character(int index, const Position &init_pos, Map *map, const string & name) : MovingObject(index, pos, map, name) {
    this ->index = index;
    this ->pos = init_pos;
    this ->map = map;
    this ->name = name;
}

// Get next position of characters
Position Character::getNextPosition(){
    Position next_pos = pos;
    int i = 0;

    if(moving_rule.empty()){
        next_pos = Position::npos;
    }
    // Go back to the start of the moving_rule
    if(currentStep == moving_rule.length()){
        currentStep = 0;
    }

    if(moving_rule[currentStep] == 'L'){
        next_pos.setCol(pos.getCol() - 1);
        i++;
    }
    else if(moving_rule[currentStep] == 'R'){
        next_pos.setCol(pos.getCol() + 1);
        i++;
    }
    else if(moving_rule[currentStep] == 'U'){
        next_pos.setRow(pos.getRow() - 1);
        i++;
    }
    else if(moving_rule[currentStep] == 'D'){
        next_pos.setRow(pos.getRow() + 1);
        i++;
    }

    if(map->isValid(next_pos, this)){
        return next_pos;
    }
    else{
        next_pos = pos;
    }
    if(i == 0){
        next_pos = Position::npos;
    }
    currentStep++;
    return next_pos;
}
Position Character::getCurrentPosition() const {
    return pos;
}

// The move of characters
void Character::move(){
    Position next_pos = getNextPosition();
    if(&next_pos == &Position::npos){
        return;
    }
    pos = next_pos;
}

// Get the kind of bag of the character
BaseBag *Character::getBag () const {
    return bag;
};


// TASK 3.5  SHERLOCK

Sherlock::Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) : Character(index, init_pos, map, "Sherlock"){
    this ->pos = init_pos;
    this->moving_rule = moving_rule;
    if(hp > 500){
        this ->hp = 500;
    }
    else{
        this ->hp = hp;
    }
    if(exp > 900){
        this ->exp = 900;
    }
    else{
        this ->exp = exp; 
    }
    bag = new SherlockBag(this);
};
void Sherlock::move() {
    Position next_pos = getNextPosition();
    if(&next_pos == &Position::npos){
        return;
    }

    pos = next_pos;
};
string Sherlock::str() const{
    // Sherlock[index=<index>;pos=<pos>;moving_rule=<moving_rule>]
    return "Sherlock[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
};

// Case of Sherlock meeting Watson
bool Sherlock::meetWatson (Watson *watson) {

    // They meet when they have the same position
    if(pos.isEqual(watson->getCurrentPosition())){
        ItemType item = PASSING_CARD;
        BaseItem *passingCard = this->bag->get(item);
        if(passingCard == nullptr || watson->getBag() == nullptr){
            return false;
        }
        while(passingCard != nullptr || watson->getBag() == nullptr){
            watson->getBag()->insert(passingCard);
            passingCard = this->bag->get(item);
        }
        return true;
    }
    return false;
}

// Cases of Sherlock meeting a Robot
bool Sherlock::meetRobot (Robot *robot) {
    if(robot == nullptr){
        return false;
    }
    if(pos.isEqual(robot->getCurrentPosition())){
        // Sherlock meets RobotS
        if(robot->getRobotType() == 1){
            if(this->getEXP() > 400){
                BaseItem *item = robot->getItem();
                this->bag->insert(item);
            }
            else{
                this->setEXP(this->getEXP() * 0.9);
            }
        }
        // Sherlock meets RobotW
        if(robot->getRobotType() == 2){
            BaseItem *item = robot->getItem();
            this->bag->insert(item);
        }
        // Sherlock meets RobotSW
        if(robot->getRobotType() == 3){
            if(this->getEXP() > 300 && this->getHP() > 335){
                BaseItem *item = robot->getItem();
                this->bag->insert(item);
            }
            else{
                this->setEXP(this->getEXP() * 0.85);
                this->setHP(this->getHP() * 0.85);
            }
        }
        // At the end of the meeting with robot, Sherlock will look in the bag for items to restore EXP or HP
        ItemType item = MAGIC_BOOK;
        BaseItem *magicBook = this->bag->get(item);
        while (magicBook != nullptr){
            magicBook->use(this, robot);
            magicBook = this->bag->get(item);
        }
        item = ENERGY_DRINK;
        BaseItem *energyDrink = this->bag->get(item);
        while (energyDrink != nullptr){
            energyDrink->use(this, robot);
            energyDrink = this->bag->get(item);
        }
        item = FIRST_AID;
        BaseItem *firstAid = this->bag->get(item);
        while (firstAid != nullptr){
            firstAid->use(this, robot);
            firstAid = this->bag->get(item);
        }
        return true;
    }
    return false;
}

// Case of Sherlock meeting RobotC
bool Sherlock::meetRobotC (Criminal *criminal, Robot *robot) {
    if(robot == nullptr){
        return false;
    }
    if(pos.isEqual(robot->getCurrentPosition())){
        if (bag->get(PASSING_CARD) != nullptr){
            BaseItem *item = this->bag->get(PASSING_CARD);
            item->use(this, robot);
        }
        else{
            if(this->getEXP() > 500){
                pos = criminal->getCurrentPosition();
            }
            else{
                BaseItem *item = robot->getItem();
                this->bag->insert(item);
            }
        }
        // At the end of the meeting with robot, Sherlock will look in the bag for items to restore EXP or HP
        ItemType item = MAGIC_BOOK;
        BaseItem *magicBook = this->bag->get(item);
        while (magicBook != nullptr){
            magicBook->use(this, robot);
            magicBook = this->bag->get(item);
        }
        item = ENERGY_DRINK;
        BaseItem *energyDrink = this->bag->get(item);
        while (energyDrink != nullptr){
            energyDrink->use(this, robot);
            energyDrink = this->bag->get(item);
        }
        item = FIRST_AID;
        BaseItem *firstAid = this->bag->get(item);
        while (firstAid != nullptr){
            firstAid->use(this, robot);
            firstAid = this->bag->get(item);
        }
        return true;
    }
    return false;
};


// TASK 3.6  WATSON

Watson::Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) : Character(index, init_pos, map, "Watson"){
    this ->pos = init_pos;
    this->moving_rule = moving_rule;
    this->exp = init_exp;
    this->hp= init_hp;
    bag = new WatsonBag(this);
};

string Watson::str() const{
    // Watson[index=<index>;pos=<pos>;moving_rule=<moving_rule>]
    return "Watson[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}
void Watson::move() {
    Position next_pos = getNextPosition();
    if(&next_pos == &Position::npos){
        return;
    }
    pos = next_pos;
};

// Case of Watson meeting Sherlock
bool Watson::meetSherlock (Sherlock *sherlock) {
    if(pos.isEqual(sherlock->getCurrentPosition())){
        ItemType item = EXCEMPTION_CARD;
        BaseItem *excemptionCard = this->bag->get(item);
        if(excemptionCard == nullptr || sherlock->getBag() == nullptr){
            return false;
        }
        while(excemptionCard != nullptr || sherlock->getBag() == nullptr){
            sherlock->getBag()->insert(excemptionCard);
            excemptionCard = this->bag->get(item);
        }
        return true;
    }
    return false;
}

// Cases of Watson meeting a Robot
bool Watson::meetRobot (Robot *robot) {
    if(robot == nullptr){
        return false;
    }
    if(pos.isEqual(robot->getCurrentPosition())){
        if (this->bag->get(EXCEMPTION_CARD) != nullptr){
            BaseItem *item = this->bag->get(EXCEMPTION_CARD);
            item->use(this, robot);
            robot = nullptr;
            return true;
        }
        else
        {
        // Watson meets RobotS
        // RobotS: Watson will not perform any actions with the robot and will not receive items held by this robot
       
        // Watson meets RobotW
            if(robot->getRobotType() == 2){
                if(this->getHP() > 350){
                    BaseItem *item = robot->getItem();
                    this->bag->insert(item);              
                }
                else{
                    this->setHP(getHP() * 0.95);
                }
            }
            // Watson meets RobotSW
            if(robot->getRobotType() == 3){
                if(this->getEXP() > 600 && this->getHP() > 165){
                    BaseItem *item = robot->getItem();
                    this->bag->insert(item);
                }
                else{
                    this->setEXP(this->getEXP() * 0.85);
                    this->setHP(this->getHP() * 0.85);
                }
            }
        }
        // At the end of the meeting with robot, Watson will look in the bag for items to restore EXP or HP
        ItemType item = MAGIC_BOOK;
        BaseItem *magicBook = this->bag->get(item);
        while (magicBook != nullptr){
            magicBook->use(this, robot);
            magicBook = this->bag->get(item);
        }
        item = ENERGY_DRINK;
        BaseItem *energyDrink = this->bag->get(item);
        while (energyDrink != nullptr){
            energyDrink->use(this, robot);
            energyDrink = this->bag->get(item);
        }
        item = FIRST_AID;
        BaseItem *firstAid = this->bag->get(item);
        while (firstAid != nullptr){
            firstAid->use(this, robot);
            firstAid = this->bag->get(item);
        }
        return true;
    }
    return false;
}

// Case of Watson meeting RobotC
bool Watson::meetRobotC (Criminal *criminal, Robot *robot) {
    if(robot == nullptr){
        return false;
    }
    if(pos.isEqual(robot->getCurrentPosition())){
        BaseItem *item1 = robot->getItem();
        this->bag->insert(item1);
        robot = nullptr;
        ItemType item = MAGIC_BOOK;
        BaseItem *magicBook = this->bag->get(item);
        while (magicBook != nullptr){
            magicBook->use(this, robot);
            magicBook = this->bag->get(item);
        }
        item = ENERGY_DRINK;
        BaseItem *energyDrink = this->bag->get(item);
        while (energyDrink != nullptr){
            energyDrink->use(this, robot);
            energyDrink = this->bag->get(item);
        }
        item = FIRST_AID;
        BaseItem *firstAid = this->bag->get(item);
        while (firstAid != nullptr){
            firstAid->use(this, robot);
            firstAid = this->bag->get(item);
        }
        return true;
    }
    return false;
};


// TASK 3.7  CRIMINAL

Criminal::Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson) : Character(index, init_pos, map, "Criminal"){
    this->sherlock = sherlock;
    this->watson = watson;
};
Position Criminal::getNextPosition() {
    Position next_pos = pos;
    int max_dist = -1;
    Position arr[4];

    arr[0] = Position(pos.getRow() - 1, pos.getCol());  // UP
    arr[1] = Position(pos.getRow(), pos.getCol() - 1);  // LEFT
    arr[2] = Position(pos.getRow() + 1, pos.getCol());  // DOWN
    arr[3] = Position(pos.getRow(), pos.getCol() + 1);  // RIGHT
    for(int i = 0; i < 4; i++){
        if(map->isValid(arr[i], this)){
            int dist = abs(arr[i].getRow() - sherlock->getCurrentPosition().getRow()) + abs(arr[i].getCol() - sherlock->getCurrentPosition().getCol()) + abs(arr[i].getRow() - watson->getCurrentPosition().getRow()) + abs(arr[i].getCol() - watson->getCurrentPosition().getCol());
            if (dist > max_dist)
            {
                max_dist = dist;
                next_pos = arr[i];
            }
            else if (dist == max_dist)
            {
                continue;
            }
        }
    }
    if (&next_pos == &pos)
    {
        next_pos = Position::npos;
    }
    check = 1;

    return next_pos;
}
string Criminal::str() const{
    //Criminal[index=<index>;pos=<pos>]
    return "Criminal[index=" + to_string(index) + ";pos=" + pos.str() + "]";
}
void Criminal::move() {
    Position next_pos = getNextPosition();
    if(&next_pos == &Position::npos){
        return;
    }
    prev_pos = pos;
    pos = next_pos;
};
Position Criminal::getPreviousPosition() {
    if(check == 0){
        // The criminal has not moved at all
        prev_pos = Position::npos;
    }
    return prev_pos;
}


// TASK 3.8  ARRAY MOVING OBJECT

ArrayMovingObject::ArrayMovingObject(int capacity){
    this ->capacity = capacity;
    this ->count = 0;

    // thực hiện cấp phát
    arr_mv_objs = new MovingObject*[capacity];  
    for (int i = 0; i < capacity; i++)
    {
        arr_mv_objs[i] = nullptr;
    }
}
ArrayMovingObject::~ArrayMovingObject(){
    if (arr_mv_objs != nullptr)
    {
        delete[] arr_mv_objs;
        arr_mv_objs = nullptr;
    }
}
bool ArrayMovingObject::isFull() const{
    return (count == capacity);
}
bool ArrayMovingObject::add(MovingObject* mv_obj){
    if(isFull()){
        return false;
    }
    arr_mv_objs[count] = mv_obj;
    count++;
    return true;
}
MovingObject *ArrayMovingObject::get(int index) const {
    if(index < 0 || index >= count){
        return nullptr;
    }
    return arr_mv_objs[index];
}
int ArrayMovingObject::size() const {
    return count;
}
string ArrayMovingObject::str() const{
    // Array[count=<count>;capacity=<capacity>;<MovingObject1>;...]
    string result = "ArrayMovingObject[count=" + to_string(count) + ";capacity=" + to_string(capacity) + ";";
    for (int i = 0; i < count; i++){
        result += get(i)->str();
        if (i != count - 1){
            result += ";";
        }
    }
    result += "]";
    return result;
}


// TASK 3.9  PROGRAM CONFIGURATION

// A FUNCTION TO TAKE THE DESIRED VALUE FROM THE INPUT STRING
void takeValue(const string &input, int &n){
    size_t index = input.find("=") + 1;
    string str = input.substr(index);
    n = stoi(str);
}
// A FUNCTION TO TAKE THE DESIRED STRING FROM THE INPUT STRING
void takeString(const string &input, string &n){
    size_t index = input.find("=") + 1;
    string str = input.substr(index);
    n = str;
}

Configuration::Configuration(const string& filepath){
    ifstream ifs(filepath);
    if(!ifs.is_open()){
        return;
    }
    string arr[15];
    for(int i = 0; i < 15; i++){
        ifs >> arr[i];
    }

    for(int i = 0; i < 15; i++){
        int n = 0;
        int &value = n;
        string s;

        if(arr[i].find("MAP_NUM_ROWS") == 0){
            takeValue(arr[i], value);
            map_num_rows = value;
        }
        else if(arr[i].find("MAP_NUM_COLS") == 0){
            takeValue(arr[i], value);
            map_num_cols = value;
        }       
        else if(arr[i].find("MAX_NUM_MOVING_OBJECTS") == 0){
            takeValue(arr[i], value);
            max_num_moving_objects = value;
        }
        else if(arr[i].find("ARRAY_WALLS") == 0){
            takeString(arr[i], s);
            if(s.length() == 2){
                // There is no walls
                num_walls = 0;
                arr_walls = nullptr;
                continue;
            }
            // FIND THE NUMBER OF WALLS
            size_t pos = s.find(";");
            int countWall = 0;
            while(pos != string::npos){
                ++countWall;
                pos = s.find(";", pos + 1);
            }
            countWall += 1;
            num_walls = countWall;
            arr_walls = new Position[num_walls];
            // READ THE PAIRS INTO THE ARRAY
            int left = s.find('('); // (1,2);(3,4);(4,5)
            int right;
            for (int j = 0; j < num_walls; j++) {
                right = s.find(')', left);
                if (right == string::npos) {
                    break;
                }
                string pair = s.substr(left, right - left + 1);
                arr_walls[j] = Position(pair);
                left = s.find('(', right);
            }
        }
        else if(arr[i].find("ARRAY_FAKE_WALLS") == 0){
            takeString(arr[i], s);
            if(s.length() == 2){
                // There is no fake walls
                num_fake_walls = 0;
                arr_fake_walls = nullptr;
                continue;
            }
            // FIND THE NUMBER OF FAKE WALLS
            size_t pos = s.find(";");
            int countFakeWall = 0;
            while(pos != string::npos){
                ++countFakeWall;
            }
            countFakeWall += 1;
            num_fake_walls = countFakeWall;
            arr_fake_walls = new Position[num_fake_walls];
            // READ THE PAIRS INTO THE ARRAY
            int left = s.find('('); // (1,2);(3,4);(4,5)
            int right;
            for (int j = 0; j < num_fake_walls; j++) {
                right = s.find(')', left);
                if (right == string::npos) {
                    break;
                }
                string pair = s.substr(left, right - left + 1);
                arr_fake_walls[j] = Position(pair);
                left = s.find('(', right);
            }
        }
        else if (arr[i].find("SHERLOCK_MOVING_RULE") == 0){
            takeString(arr[i], s);
            sherlock_moving_rule = s;
        }
        else if (arr[i].find("SHERLOCK_INIT_POS") == 0){
            takeString(arr[i], s);
            sherlock_init_pos = Position(s);
        }
        else if(arr[i].find("SHERLOCK_INIT_HP") == 0){
            takeValue(arr[i], value);
            sherlock_init_hp = value;
        }       
        else if(arr[i].find("SHERLOCK_INIT_EXP") == 0){
            takeValue(arr[i], value);
            sherlock_init_exp = value;
        }
        else if (arr[i].find("WATSON_MOVING_RULE") == 0){
            takeString(arr[i], s);
            watson_moving_rule = s;
        }
        else if (arr[i].find("WATSON_INIT_POS") == 0){
            takeString(arr[i], s);
            watson_init_pos = Position(s);
        }
        else if(arr[i].find("WATSON_INIT_HP") == 0){
            takeValue(arr[i], value);
            watson_init_hp = value;
        }       
        else if(arr[i].find("WATSON_INIT_EXP") == 0){
            takeValue(arr[i], value);
            watson_init_exp = value;
        }
        else if (arr[i].find("CRIMINAL_INIT_POS") == 0){
            takeString(arr[i], s);
            criminal_init_pos = Position(s);
        }
        else if (arr[i].find("NUM_STEPS") == 0){
            takeValue(arr[i], value);
            num_steps = value;
        }
    }
    ifs.close();
}
Configuration::~Configuration(){
    delete[] arr_walls;
    delete[] arr_fake_walls;
}

string Configuration::str() const{
// Configuration[
// MAP_NUM_ROWS=10
// MAP_NUM_COLS=10
// MAX_NUM_MOVING_OBJECTS=10
// NUM_WALLS=3
// ARRAY_WALLS=[(1,2);(2,3);(3,4)]
// NUM_FAKE_WALLS=1
// ARRAY_FAKE_WALLS=[(4,5)]
// SHERLOCK_MOVING_RULE=RUU
// SHERLOCK_INIT_POS=(1,3)
// SHERLOCK_INIT_HP=250
// SHERLOCK_INIT_EXP=500
// WATSON_MOVING_RULE=LU
// WATSON_INIT_POS=(2,1)
// WATSON_INIT_HP=300
// WATSON_INIT_EXP=350
// CRIMINAL_INIT_POS=(7,9)
// NUM_STEPS=100
// ]
    string result;
    result = "Configuration[\n";
    result += "MAP_NUM_ROWS=" + to_string(map_num_rows) + "\n";
    result += "MAP_NUM_COLS=" + to_string(map_num_cols) + "\n";
    result += "MAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) + "\n";
    result += "NUM_WALLS=" + to_string(num_walls) + "\n";
    result += "ARRAY_WALLS=[";
    for(int i = 0; i < num_walls; i++){
        result += (arr_walls[i]).str();
        // Make sure to delete the trailing semicolon
        if(i < num_walls - 1){
            result += ";";
        }
    }
    result += "]\n";
    result += "NUM_FAKE_WALLS=" + to_string(num_fake_walls) + "\n";
    result += "ARRAY_FAKE_WALLS=[";
    for(int i = 0; i < num_fake_walls; i++){
        result += (arr_fake_walls)[i].str();
        // Make sure to delete the trailing semicolon
        if(i < num_fake_walls - 1){
            result += ";";
        }
    }   
    result += "]\n";
    result += "SHERLOCK_MOVING_RULE=" + sherlock_moving_rule + "\n";
    result += "SHERLOCK_INIT_POS=" + (sherlock_init_pos).str() + "\n";
    result += "SHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) + "\n";
    result += "SHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) + "\n";
    result += "WATSON_MOVING_RULE=" + watson_moving_rule + "\n";
    result += "WATSON_INIT_POS=" + (watson_init_pos).str() + "\n";
    result += "WATSON_INIT_HP=" + to_string(watson_init_hp) + "\n";
    result += "WATSON_INIT_EXP=" + to_string(watson_init_exp) + "\n";
    result += "CRIMINAL_INIT_POS=" + (criminal_init_pos).str() + "\n";
    result += "NUM_STEPS=" + to_string(num_steps) + "\n";
    result += "]";   
    return result; 
}


// TASK 3.10  ROBOT

// CONSTRUCTOR FOR CLASS ROBOT
Robot::Robot(int index, const Position &init_pos, Map *map, RobotType robot_type) : MovingObject(index, init_pos, map, "Robot") {
    this ->criminal = criminal;
    this ->robot_type = robot_type;

    // Gọi vị trí tạo ra robot có toạ độ là (i,j) với i là chỉ số hàng, j là chỉ số cột. 
    // Với p = i ∗ j. Gọi s số chủ đạo của p . Ta định nghĩa số chủ đạo của một số là 
    // giá trị tổng các chữ số, cho đến khi giá trị tổng đó là số có 1 chữ số
    int p = init_pos.getRow() * init_pos.getCol();
    int s = 0;
    if(p == 0){
        s = 0;
    }
    s = p % 9;
    if(s == 0){
        s = 9;
    }
    int t = (init_pos.getRow() * 11 + init_pos.getCol()) % 4;
    if (s >= 0 && s <= 1){
        *item = MagicBook();
    }
    else if (s >= 2 && s <= 3){
        *item = EnergyDrink();
    }
    else if (s >= 4 && s <= 5){
        *item = FirstAid();
    }
    else if (s >= 6 && s <= 7){
        *item = ExcemptionCard();
    }
    else if (s >= 8 && s <= 9){
        switch(t){
        case 0:
            *item = PassingCard("RobotS");
            break;
        case 1:
            *item = PassingCard("RobotC");
            break;
        case 2:
            *item = PassingCard("RobotSW");
            break;
        case 3:
            *item = PassingCard("all");
            break;
        default:
            break;
        }
    }
};

// To know the number of robots existing
int Robot::countRobot = 0;

Robot *Robot::createRobot (int index, Map *map, Sherlock *sherlock, Watson *watson, Criminal *criminal) {
    Robot *robot;
    Position pos = criminal->getCurrentPosition();
    if(countRobot == 0){
        robot = new RobotC(0, pos, map, criminal);
    }
    else{
        int dist_toS = abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(pos.getCol() - sherlock->getCurrentPosition().getCol());
        int dist_toW = abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());
        if (dist_toS < dist_toW){
            robot = new RobotS(0, pos, map, criminal, sherlock);
        }
        else if (dist_toW < dist_toS){
            robot = new RobotW(0, pos, map, criminal, watson);
        }
        else{
            robot = new RobotSW(0, pos, map, criminal, sherlock, watson);
        }
    }
    return robot;
}
int Robot::getRobotType() const {
    return robot_type;
};
Position Robot::getCurrentPosition() const {
    return pos;
}
void Robot::move() {
    Position next_pos = getNextPosition();
    if(&next_pos == &Position::npos){
        return;
    }
    pos = next_pos;
}
BaseItem *Robot::getItem() {
    return item;
}

// CONSTRUCTOR FOR CLASS ROBOTC
RobotC::RobotC ( int index , const Position & init_pos , Map * map , Criminal * criminal ) : Robot(index, init_pos, map, C) {
    this ->criminal = criminal;
    this ->name = "RobotC";
    countRobot++;
} ;
Position RobotC::getNextPosition() {
    Position next_pos = criminal->getPreviousPosition();
    if(&next_pos == &pos){
        next_pos = Position::npos;
    }
    return next_pos;
}
int RobotC::getDistance(Character * character) const {
    //|x1 − x2| + |y1 − y2|
    return abs(pos.getRow() - character->getCurrentPosition().getRow()) + abs(pos.getCol() - character->getCurrentPosition().getCol());
}
string RobotC::str() const {
    // Robot[pos=<pos>;type=<robot_type>;dist=<dist>]
    return "Robot[pos=" + pos.str() + ";type=C;dist=]";
}

//CONSTRUCTOR FOR CLASS ROBOT_SHERLOCK
RobotS::RobotS ( int index , const Position & init_pos , Map * map , Criminal * criminal , Sherlock * sherlock) : Robot(index, init_pos, map, S) {
    this ->criminal = criminal;
    this ->sherlock = sherlock;
    this ->name = "RobotS";
    countRobot++;
};
Position RobotS::getNextPosition() {
    Position next_pos = pos;
    int min_dist = 1000000;
    Position arr[4];
    arr[0] = Position(pos.getRow() - 1, pos.getCol());
    arr[1] = Position(pos.getRow(), pos.getCol() + 1);
    arr[2] = Position(pos.getRow() + 1, pos.getCol());
    arr[3] = Position(pos.getRow(), pos.getCol() - 1);

    for (int i = 0; i < 4; i++){
        if (map->isValid(arr[i], this)){
            int dist = abs(arr[i].getRow() - sherlock->getCurrentPosition().getRow()) + abs(arr[i].getCol() - sherlock->getCurrentPosition().getCol());
            if (dist < min_dist){
                min_dist = dist;
                next_pos = arr[i];
            }
            else if (dist == min_dist){
                continue;
            }
        }
    }
    if (&next_pos == &pos){
        next_pos = Position::npos;
    }
    return next_pos;
};
int RobotS::getDistance() const {
    // |x1 − x2| + |y1 − y2|
    return abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(pos.getCol() + sherlock->getCurrentPosition().getCol());
};
string RobotS::str() const {
    // Robot[pos=<pos>;type=<robot_type>;dist=<dist>]
    return "Robot[pos=" + pos.str() + ";type=S;dist=" + to_string(getDistance()) + "]";
}

//CONSTRUCTOR FOR CLASS ROBOT_WATSON
RobotW::RobotW ( int index , const Position & init_pos , Map * map , Criminal * criminal , Watson * watson) : Robot(index, init_pos, map, W) {
    this->watson = watson;
    this->criminal = criminal;
    this->name = "RobotW";
    countRobot++;
};
Position RobotW::getNextPosition() {
    Position next_pos = pos;
    int min_dist = 1000000;
    Position arr[4];
    arr[0] = Position(pos.getRow() - 1, pos.getCol());
    arr[1] = Position(pos.getRow(), pos.getCol() + 1);
    arr[2] = Position(pos.getRow() + 1, pos.getCol());
    arr[3] = Position(pos.getRow(), pos.getCol() - 1);

    for (int i = 0; i < 4; i++){
        if (map->isValid(arr[i], this)){
            int dist = abs(arr[i].getRow() - watson->getCurrentPosition().getRow()) + abs(arr[i].getCol() - watson->getCurrentPosition().getCol());
            if (dist < min_dist){
                min_dist = dist;
                next_pos = arr[i];
            }
            else if (dist == min_dist){
                continue;
            }
        }
    }
    if (&next_pos == &pos){
        next_pos = Position::npos;
    }
    return next_pos;
}

int RobotW::getDistance() const {
    return abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());
};

string RobotW::str() const {
    // Robot[pos=<pos>;type=<robot_type>;dist=<dist>]
    return "Robot[pos=" + pos.str() + ";type=W;dist=" + to_string(getDistance()) + "]";
}

//CONSTRUCTOR FOR CLASS ROBOT_SHERLOCK&WATSON
RobotSW::RobotSW ( int index , const Position & init_pos , Map * map , Criminal * criminal , Sherlock * sherlock , Watson * watson) : Robot(index, init_pos, map, SW) {
    this->criminal = criminal;
    this->sherlock = sherlock;
    this->watson = watson;
    this->name = "RobotSW";
    countRobot++;
};
Position RobotSW::getNextPosition() {
    Position next_pos = pos;
    int min_dist = 1000000;
    Position arr[4];
    arr[0] = Position(pos.getRow() - 2, pos.getCol());
    arr[1] = Position(pos.getRow(), pos.getCol() + 2);
    arr[2] = Position(pos.getRow() + 2, pos.getCol());
    arr[3] = Position(pos.getRow(), pos.getCol() - 2);

    for (int i = 0; i < 4; i++){
        if (map->isValid(arr[i], this)){
            int dist = abs(arr[i].getRow() - sherlock->getCurrentPosition().getRow()) + abs(arr[i].getCol() - sherlock->getCurrentPosition().getCol()) + abs(arr[i].getRow() - watson->getCurrentPosition().getRow()) + abs(arr[i].getCol() - watson->getCurrentPosition().getCol());
            if (dist < min_dist){
                min_dist = dist;
                next_pos = arr[i];
            }
            else if (dist == min_dist){
                continue;
            }
        }
    }
    if (&next_pos == &pos){
        next_pos = Position::npos;
    }
    return next_pos;
}
int RobotSW::getDistance() const {
    return abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(pos.getCol() - sherlock->getCurrentPosition().getCol()) + abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());
};
string RobotSW::str() const {
    // Robot[pos=<pos>;type=<robot_type>;dist=<dist>]
    return "Robot[pos=" + pos.str() + ";type=SW;dist=" + to_string(getDistance()) + "]";
}

// 3.11  BASE ITEM

// CLASS BASE_ITEM
bool BaseItem::canUse ( Character * obj , Robot * robot ) {
    if((robot->getItem())->canUse(obj, robot)){
        return true;
    };
    return false;
};

// CLASS MAGICBOOK
bool MagicBook::canUse(Character *obj, Robot *robot) {
    if(obj->getEXP() <= 350){
        return true;
    }
    return false;
};
void MagicBook::use(Character *obj, Robot *robot) {
    if(canUse(obj, robot)){
        obj->setEXP(obj->getEXP() * 1.25);
    }
};
ItemType MagicBook::getItemType() const {
    return MAGIC_BOOK;
};

// CLASS ENERGYDRINK
bool EnergyDrink::canUse(Character *obj, Robot *robot) {
    if(obj->getHP() <= 100){
        return true;
    }
    return false;
};
void EnergyDrink::use(Character *obj, Robot *robot) {
    if(canUse(obj, robot)){
        obj->setHP(obj->getHP() * 1.2);
    }
};
ItemType EnergyDrink::getItemType() const {
    return ENERGY_DRINK;
};

// CLASS FIRSTAID
bool FirstAid::canUse(Character *obj, Robot *robot) {
    if(obj->getHP() <= 100 || obj->getEXP() <= 350){
        return true;
    }
    return false;
};
void FirstAid::use(Character *obj, Robot *robot) {
    if(canUse(obj, robot)){
        obj->setHP(obj->getHP() * 1.5);
    }
};
ItemType FirstAid::getItemType() const {
    return FIRST_AID;
};

// CLASS EXCEMPTIONCARD
// Only Sherlock can use this card and when Sherlock’s hp is an odd number
bool ExcemptionCard::canUse(Character *obj, Robot *robot) {
    if(obj->getName() == "Sherlock" && obj->getHP() % 2 != 0){
        return true;
    }
    return false;
};
void ExcemptionCard::use(Character *obj, Robot *robot) {
    obj->setEXP(obj->getEXP());
    obj->setHP(obj->getHP());
};
ItemType ExcemptionCard::getItemType() const {
    return EXCEMPTION_CARD;
};

// CLASS PASSINGCARD
PassingCard::PassingCard (const string &challenge) {
    this ->challenge = challenge;
};
// Only Watson can use this card and when Watson’s hp is an even number
bool PassingCard::canUse(Character *obj, Robot *robot) {
    if(obj->getName() == "Watson" && obj->getHP() % 2 == 0){
        return true;
    }
    return false;
};
void PassingCard::use(Character *obj, Robot *robot) {
    if(challenge == robot->getName()){
        BaseItem *item = robot->getItem();
        obj->getBag()->insert(item);
    }
    else{
        obj->setEXP(obj->getEXP() - 50);
        BaseItem *item = robot->getItem();
        obj->getBag()->insert(item);
    }
};
ItemType PassingCard::getItemType() const {
    return PASSING_CARD;
};


// TASK 3.12  BASE BAG

// Constructors for class BaseBag
bool BaseBag::insert (BaseItem * item) {
    if (obj->getName() == "Sherlock"){
        if (size <= 13){
            Node *newNode = new Node(item, head);
            head = newNode;
            size++;
            return true;
        }
    }
    if (obj->getName() == "Watson"){
        if (size <= 15){
            Node *newNode = new Node(item, head);
            head = newNode;
            size++;
            return true;
        }
    }
    return false;
}

// Find the item in the bag that can be used and is closest to the top of the bag
BaseItem *BaseBag::get() {
    if (size == 0) {
        return nullptr;
    }

    Node *previous = head;
    Node *current = head;

    while(current != nullptr){
        if(current->item->canUse(obj, nullptr)){
            // If the item is at the top of the bag
            if(current == head){
                head = current->next;
            }
            else{
                // If the item is somewhere in the bag
                while (previous->next != current){
                    previous = previous->next;
                }
                previous->next = current->next;
            }
            BaseItem *item = current->item;
            size--;
            delete current;
            return item;
        }
        current = current->next;
    }
    return nullptr;
};

// If in the inventory there is an item of the type to be used, 
// the character can use this item by reversing its position with first item
// (if it is not the first item) and remove it from the list
BaseItem *BaseBag::get ( ItemType itemType ) {
    if(size == 0){
        return nullptr;
    }

    Node *previous = nullptr;
    Node *current = head;

    while(current != nullptr){
        if(current->item->getItemType() == itemType){
            // If the item is at the top of the bag
            if(previous = nullptr){
                head = current->next;
            }
            else{
                // If the item is somewhere in the bag
                previous->next = current->next;
            }
            BaseItem *item = current->item;
            size--;
            delete current;
            return item;
        }
        previous = current;
        current = current->next;
    }
    return nullptr;
} ;

// Bag[count=<c>;<list_items>]
string BaseBag::str() const {
    string result = "Bag[count=" + to_string(size) + ";";
    Node *temp = head;
    while (temp != nullptr) {
        int itemName = temp->item->getItemType();
        if (itemName == (MAGIC_BOOK)) {
            result += "MagicBook";
        } else if (itemName == (ENERGY_DRINK)) {
            result += "EnergyDrink";
        } else if (itemName == (ItemType::FIRST_AID)) {
            result += "FirstAid";
        } else if (itemName == (ItemType::EXCEMPTION_CARD)) {
            result += "ExcemptionCard";
        } else if (itemName == (ItemType::PASSING_CARD)) {
            result += "PassingCard";
        } 
        if (temp->next != nullptr) {
            result += ",";
        }
        temp = temp->next;
    }
    result += "]";
    return result;
};

// Constructor for class SherlockBag
SherlockBag::SherlockBag (Sherlock *sherlock) : BaseBag (13) {
    this ->obj = sherlock;
};

// Constructor for class WatsonBag
WatsonBag::WatsonBag (Watson *watson) : BaseBag (15) {
    this ->obj = watson;
};


// TASK 3.13  STUDYINPINKPROGRAM
StudyInPinkProgram::StudyInPinkProgram(const string & config_file_path) {
    ifstream ifs(config_file_path);
    if(!ifs.is_open()){
        return;
    }
    config = new Configuration(config_file_path); 

    // Sau khi khởi tạo thì lần lượt thêm vào criminal, sherlock, watson bằng phương thức add
    map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
    sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
    watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
    criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);
    arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
    arr_mv_objs->add(criminal);
    arr_mv_objs->add(sherlock);
    arr_mv_objs->add(watson);

    ifs.close();
};
bool StudyInPinkProgram::isStop () const {
    if (sherlock->getHP() == 1 || watson->getHP() == 1 || criminal->getCurrentPosition().isEqual(sherlock->getCurrentPosition()) || criminal->getCurrentPosition().isEqual(watson->getCurrentPosition())){
        return true;
    }
    return false;
};
void StudyInPinkProgram::printResult() const {
    if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
        cout << "Sherlock caught the criminal" << endl;
    }
    else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
        cout << "Watson caught the criminal" << endl;
    }
    else {
        cout << "The criminal escaped" << endl;
    }
};
void StudyInPinkProgram::printStep(int si) const {
    cout << "Step: " << setw(4) << setfill('0') << si
        << "--"
        << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
};
void StudyInPinkProgram::run (bool verbose) {
    Robot *robot = nullptr;
    int criminalStep = 0;
    bool stop = false;

    for (int istep = 0; istep < config->num_steps; ++istep) {

        for (int i = 0; i < arr_mv_objs->size(); ++i) {

            Position curr_pos = arr_mv_objs->get(i)->getCurrentPosition();
            arr_mv_objs->get(i)->move();

            if (arr_mv_objs->get(i)->getName() == "Sherlock"){
                sherlock->meetWatson(watson);
                sherlock->meetRobot(robot);
                sherlock->meetRobotC(criminal, robot);
            }

            if (arr_mv_objs->get(i)->getName() == "Watson"){
                watson->meetSherlock(sherlock);                                                                
                watson->meetRobot(robot);
                watson->meetRobotC(criminal, robot);
            }

            if (arr_mv_objs->get(i)->getName() == "Criminal"){
                if (criminalStep % 3 == 0 && criminalStep != 0){
                    robot = Robot::createRobot(Robot::countRobot, map, sherlock, watson, criminal);
                    arr_mv_objs->add(robot);
                    criminalStep++;
                }
            }

            if (verbose) {
                printStep(istep);
                if(!arr_mv_objs->get(i)->getCurrentPosition().isEqual(curr_pos)){
                    cout << arr_mv_objs->get(i)->getName() << " moved!" << endl;
                }

                if(arr_mv_objs->get(i)->getName() == "Sherlock"){
                    if(sherlock->meetWatson(watson)){
                        cout << "Sherlock met Watson!" << endl;
                    }
                    if(sherlock->meetRobot(robot) || sherlock->meetRobotC(criminal, robot)){
                        cout << "Sherlock encounters a Robot and performs challenge with Robot" << endl;
                    }
                }

                if(arr_mv_objs->get(i)->getName() == "Watson"){
                    if(watson->meetSherlock(sherlock)){
                        cout << "Watson met Sherlock!" << endl;
                    }
                    if(watson->meetRobot(robot) || watson->meetRobotC(criminal, robot)){
                        cout << "Watson encounters a Robot and performs challenge with Robot" << endl;
                    }
                }
            }

            if (isStop()) {
                stop = true;
                break;
            }
        }

        if(stop) {
            break;
        }
        
    }

    printResult();
}


//////////////////////////////////////////////// 
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////