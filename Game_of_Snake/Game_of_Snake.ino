#include <TM74HC595Display.h>
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


int pinCS = 9; // CLK - 15. DIN - 16.
int numberOfHorizontalDisplays = 1;
int numberOfVerticalDisplays = 1;

int SCLK = 2;
int RCLK = 3;
int DIO = 4;

TM74HC595Display disp(SCLK, RCLK, DIO);
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

// Функция для очистки матрицы
void clear_matrix(){
  for (int i = 0; i < 8; i++){ 
  for (int j = 0; j < 8; j++){
  matrix.drawPixel(i, j, LOW);}}
  matrix.write();
}


// Функция для заполнения матрицы
void fill_matrix(){
  for (int i = 0; i < 8; i++){ 
  for (int j = 0; j < 8; j++){
  matrix.drawPixel(i, j, HIGH);}}
  matrix.write();
}


void game_over(){
  clear_matrix();
  delay(1000);
  fill_matrix();
  delay(1000);
  clear_matrix();
  delay(1000);
  disp.digit4(0, 1);
  game_speed = 200;
  Serial.println();
  Serial.println("New Game Started");
  Serial.println();
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
        Serial.print("Length: ");
        Serial.println(lenght);
      }
  
      void clear_body(){
        delete[] parts;
        lenght = 0;
        add_part(4,4,true, true);
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
        matrix.drawPixel(body.parts[part].curr_coords[Y], body.parts[part].curr_coords[X], HIGH);
      } 
      matrix.write();
    };    

    // Функция добавления новой части тела
    void add_part(){
      if (ready_to_prolong && got_aims_X.count() > 0 && got_aims_Y.count() > 0){
        int new_x = got_aims_X.pop();
        int new_y = got_aims_Y.pop();
        
        Serial.print("Added new part: ");
        Serial.print(new_x);
        Serial.print(" , ");
        Serial.println(new_y);
        
        body.add_part(new_x, new_y, false, true);
        if (got_aims_X.count() == 0 && got_aims_Y.count() == 0) ready_to_prolong = false;
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
      Serial.print("Caught aim: ");
      Serial.print(aim_dot[X]);
      Serial.print(" , ");
      Serial.println(aim_dot[Y]);
      
      got_aims_X.push(aim_dot[X]);
      got_aims_Y.push(aim_dot[Y]);
    }
    
    // Функция фиксации захвата цели
    void get_the_aim(){
      game_score++;
      game_speed-= 2;
      add_aim_to_pool();
      has_aim = false;    
    }
};

// Объявление объектов Змеи и Цели
AimScore aim = AimScore();
Snake snake = Snake();

// Функция проверки столкновения с самим собой (змеи)
void check_self_collision(){
  for (int i = 2; i < snake.body.lenght; i ++){
    if (snake.body.parts[0].curr_coords[X] == snake.body.parts[i].curr_coords[X] && snake.body.parts[0].curr_coords[Y] == snake.body.parts[i].curr_coords[Y])
    {
      for (int i = 0; i < 17; i++){
        matrix.drawPixel(snake.body.parts[0].curr_coords[Y], snake.body.parts[0].curr_coords[X], i%2==0);
        matrix.write();
        delay(100);
      }
      
      delay(3000);
      game_over();
      aim.game_score = 0;
      snake.body.clear_body();
      snake.ready_to_prolong = false;
    }
  }
}

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
}

void setup() {
  pinMode(debug_button, INPUT);
  digitalWrite(debug_button, HIGH);
  Serial.begin(9600);
  matrix.setIntensity(4);
  clear_matrix();
  disp.digit4(0, 1);
  Serial.println();
  Serial.println("New Game Started");
  Serial.println();
}

void loop() {
  
  listen_events();
  disp.digit4(aim.game_score, 1);
  
  if ((millis() - lastTime) > game_speed) {
    snake.advance();
    
    // Голова столкнулась с целью
    if (snake.body.parts[0].curr_coords[X] == aim.aim_dot[X] && snake.body.parts[0].curr_coords[Y] == aim.aim_dot[Y]) aim.get_the_aim();
    // Хвост столкнулся с целью
    if ((snake.body.parts[snake.body.lenght-1].curr_coords[X] == got_aims_X.peek() && snake.body.parts[snake.body.lenght-1].curr_coords[Y] == got_aims_Y.peek())) snake.ready_to_prolong = true;
    // Хвост вышел из ячейки с целью
    if ((snake.body.parts[snake.body.lenght-1].old_coords[X] == got_aims_X.peek() || snake.body.parts[snake.body.lenght-1].old_coords[Y] == got_aims_Y.peek())) snake.add_part();
  
    check_self_collision();
    
    lastTime = millis();
  }
  snake.draw();
  
  if (!aim.has_aim){
    aim.guess_aim(snake);
    aim.has_aim = true;   
  }
  aim.draw_aim();
    
  if (digitalRead(debug_button)==LOW){
  }
}
