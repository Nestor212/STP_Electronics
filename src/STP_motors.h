



class motor {
    public:
        motor();

        void init(int contPin, int dirPin);
        void setSpeed(int speed);
        void setDir(int dir);
        int getSpeed();
        int getDir();
        int dirPin;
        int controlPin;


    private:
        int direction;
        int speed;
};