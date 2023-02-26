#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include "Queue.h":

const int X = 0;
const int Y = 1;

Queue<int> got_aims_X(10);
Queue<int> got_aims_Y(10);

unsigned long lastTime = 0;

int game_speed = 200;
const int X_pin = 0; 
const int Y_pin = 1; 
const int debug_button = 5;


int pinCS = 9;
int numberOfHorizontalDisplays = 1;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

// Функция для очистки матрицы
void clear_matrix(){
  for (int i = 0; i < 8; i++){ 
  for (int j = 0; j < 8; j++){
  matrix.drawPixel(i, j, LOW);}}
  matrix.write();
}

class Snake : public Printable{
      
  class Parts{
    
    class Part {
      
      public:
        Part(int x = 3, int y = 3){
          update_curr_coords(x ,y);
        };
      
      public:
        int curr_coords[2];
        int old_coords[2];
        
        bool is_head = false;
        bool is_tail = false;
              
        void update_curr_coords(int x, int y){
          old_coords[X] = curr_coords[X];
          old_coords[Y] = curr_coords[Y];
          
          curr_coords[X] = x;
          curr_coords[Y] = y;}  
        
    };

    // Parts
    public:      
      Part parts[64];
      int lenght = 0;
    
    public:
      Parts(){
        Serial.println("Invoked");
      };
          
    public:  
      void add_part(int x, int y, bool is_head, bool is_tail){
        Part part = Part(x, y);
        
        if (is_head){
          part.is_head = true;
          part.is_tail = true;
        }
        if (is_tail){
          parts[lenght-1].is_tail = false;
          part.is_tail = true;
        }
        
        parts[lenght] = part;
        lenght++;
      }
  
      void clear_body(){
        delete[] parts;
      }
  };

  // Snake
  public:
    Parts body = Parts();
    int move_direction[2] = {1,0};
    bool ready_to_prolong = false;
    
  public:
      Snake() {
        body.add_part(4,4,true, true);
      }
  public:
    // Функция шага змеи (передвижения)
    void advance(){
      for (int part = 0; part < body.lenght; part++){
        
        // Достижение края матрицы  
        for (int coord = 0; coord < 2; coord++){
          if (body.parts[part].curr_coords[coord]==7 && move_direction[coord] == 1){
            body.parts[part].curr_coords[coord] = -1;
          }
          if (body.parts[part].curr_coords[coord]==0 && move_direction[coord] == -1){
            body.parts[part].curr_coords[coord] = 8;
          } 
        }
        
        
        // Продвижение змеи
        if (part == 0){
          int new_x = body.parts[part].curr_coords[X] + move_direction[X];
          int new_y = body.parts[part].curr_coords[Y] + move_direction[Y];
          body.parts[part].update_curr_coords(new_x, new_y);
        }
        else{
          int new_x = body.parts[part-1].old_coords[X];
          int new_y = body.parts[part-1].old_coords[Y];
          body.parts[part].update_curr_coords(new_x, new_y);
        }
      }  
    };

    // Функция вывод змейки на матрицу
    void draw(){
      clear_matrix();
      for (int part = 0; part < body.lenght; part++){
        matrix.drawPixel(body.parts[part].curr_coords[Y], body.parts[part].curr_coords[X], HIGH);}
      matrix.write();
    };    

    // Функция добавления новой части тела
    void add_part(){
      if (ready_to_prolong){
        int new_x = got_aims_X.pop();
        int new_y = got_aims_Y.pop();
        body.add_part(new_x, new_y, false, true);
        ready_to_prolong = false;
      }
    }
    
      size_t printTo(Print& p) const {
           size_t r = 0;

           r += p.print("X direction: ");
           r += p.print(move_direction[X]);
           r += p.print(" | Y direction: ");
           r += p.print(move_direction[Y]);
           
           return r;
     }    
};

// Класс цели и его данных
class AimScore {
  
  public:
    int aim_dot[2];
    bool has_aim = false;
    int game_score = 0;

    // Функция задания новой цели
    void guess_aim(Snake& snake){
      clear_matrix();
      int rand_x = random(8);
      int rand_y = random(8);
      for (int part = 0; part < snake.body.lenght; part++){
        if (snake.body.parts[part].curr_coords[X] != rand_x && snake.body.parts[part].curr_coords[Y] != rand_y){
          aim_dot[X] = rand_x; aim_dot[Y] = rand_y;    
        }
      }
    }

    // Функция вывода цели на матрицу
    void draw_aim(){
      matrix.drawPixel(aim_dot[Y], aim_dot[X], HIGH);
      matrix.write();
    }

    // Функция добавления пойманной цели в массив
    void add_aim_to_pool(){
      got_aims_X.push(aim_dot[X]);
      got_aims_Y.push(aim_dot[Y]);
    }
    
    // Функция фиксации захвата цели
    void get_the_aim(){
      Serial.println("Caught the aim");
      game_score++;
      game_speed-= 5;
      add_aim_to_pool();
      has_aim = false;    
    }
};

// Объявление объектов Змеи и Цели
AimScore aim = AimScore();
Snake snake = Snake();

// Функция изменения направления движения змеи
void change_direction(int x, int y){
  if (abs(x)){snake.move_direction[X]=x; snake.move_direction[Y]=0;};
  if (abs(y)){snake.move_direction[Y]=y; snake.move_direction[X]=0;};
}


// Функция регистрации событии в игре
void listen_events(){
  if (analogRead(X_pin) <= 100 && snake.move_direction[X] != 1) change_direction(-1,0);     
  if (analogRead(X_pin) >= 923 && snake.move_direction[X] != -1) change_direction(1,0); 
  if (analogRead(Y_pin) <= 100 && snake.move_direction[Y] != -1) change_direction(0,1);     
  if (analogRead(Y_pin) >= 923 && snake.move_direction[Y] != 1) change_direction(0,-1);

  // Голова столкнулась с целью
  if (snake.body.parts[0].curr_coords[X] == aim.aim_dot[X] && snake.body.parts[0].curr_coords[Y] == aim.aim_dot[Y]) aim.get_the_aim();

  // Хвост столкнулся с целью
  if ((snake.body.parts[snake.body.lenght-1].curr_coords[X] == got_aims_X.peek() && snake.body.parts[snake.body.lenght-1].curr_coords[Y] == got_aims_Y.peek())) snake.ready_to_prolong = true;
  // Хвост вышел из ячейки с целью
  if ((snake.body.parts[snake.body.lenght-1].old_coords[X] == got_aims_X.peek() || snake.body.parts[snake.body.lenght-1].old_coords[Y] == got_aims_Y.peek())) snake.add_part();
}

void setup() {
  pinMode(debug_button, INPUT);
  digitalWrite(debug_button, HIGH);
  Serial.begin(9600);
  matrix.setIntensity(4);
  clear_matrix();
}

void loop() {
  
  listen_events();
  if ((millis() - lastTime) > game_speed) {
    snake.advance();
    Serial.println(snake.body.lenght);
    lastTime = millis();
  }
  snake.draw();
  
  if (!aim.has_aim){
    aim.guess_aim(snake);
    aim.has_aim = true;   
  }
  aim.draw_aim();
    
  if (digitalRead(debug_button)==LOW){
    Serial.println(snake);
  }
}
