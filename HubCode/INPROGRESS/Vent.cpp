class Vent {
    private:
        double temp;
        int servo;
        int sig;
    
    public:
        Vent(double temp, int servo);
        double getTemp(void);
        double getServo(void);
        void setTemp(double temp);
        void setServo(int servo);
};

Vent::Vent(double sig){
    this->sig = sig;
}

double Vent::getTemp(void){
    return temp;
}

double Vent::getServo(void){
    return servo;
}

void Vent::setServo(int servo){
    this->servo = servo;
}

void Vent::setTemp(double temp){
    this->temp = temp;
}

