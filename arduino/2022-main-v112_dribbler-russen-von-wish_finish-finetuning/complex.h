
struct Complex {
  
  double Re, Im, phi, rad;  //phi is in I=[-PI;PI] und winkel im uhrzeiger sinn = positiv (entgegen der mathematischen weise)

  Complex(): Re(0), Im(0) {
    this->update_PhiRad();
  }
  
  Complex(double a, double b): Re(a), Im(b) {
    this->update_PhiRad();
  }

  Complex clone() {
    return Complex(this->Re, this->Im);
  }

  static Complex clone(const Complex& z) {
    return Complex(z.Re, z.Im);
  }


  void debug_println() const {
    Serial.println("z = "+String(this->Re)+" + "+String(this->Im)+"i = "+String(this->rad)+" * e^"+String(this->phi/PI)+"PI*i");
  }

  String debug_string() const {
    return "z = "+String(this->Re)+" + "+String(this->Im)+"i = "+String(this->rad)+" * e^"+String(this->phi/PI)+"PI*i";
  }



  void update_PhiRad() {
    double angle = atan2(Im,Re);
    //this->phi = (angle >= 0 ? angle : 2*PI+angle);
    this->phi = angle;
    this->rad = sqrt((Re*Re) + (Im*Im));
  }

  void update_ReIm() {
    this->Re = rad*cos(this->phi);
    this->Im = rad*sin(this->phi);
  }


  void add(const Complex& z) {
    this->Re += z.Re;
    this->Im += z.Im;
    this->update_PhiRad();
  }

  void operator +=(const Complex& z) {
    this->add(z);
  }

  void add(const double& n) {
    this->Re += n;
    this->update_PhiRad();
  }

  void operator +=(const double& n) {
    this->add(n);
  }
  
  static Complex add(const Complex& z1, const Complex& z2) {
    return Complex(z1.Re + z2.Re, z1.Im + z2.Im);
  }

  Complex operator +(const Complex& z) {
    return Complex::add(*this, z);
  }
  
  static Complex add(const Complex& z, const double& n) {
    return Complex(z.Re + n, z.Im);
  }

  Complex operator +(const double& n) {
    return Complex::add(*this, n);
  }



  void subtract(const Complex& z) {
    this->Re -= z.Re;
    this->Im -= z.Im;
    this->update_PhiRad();
  }

  void operator -=(const Complex& z) {
    this->subtract(z);
  }

  void subtract(const double& n) {
    this->Re -= n;
    this->update_PhiRad();
  }

  void operator -=(const double& n) {
    this->subtract(n);
  }
  
  static Complex subtract(const Complex& z1, const Complex& z2) {
    return Complex(z1.Re - z2.Re, z1.Im - z2.Im);
  }

  Complex operator -(const Complex& z) {
    return Complex::subtract(*this, z);
  }

  static Complex subtract(const Complex& z1, const double& n) {
    return Complex(z1.Re - n, z1.Im);
  }

  Complex operator -(const double& n) {
    return Complex::subtract(*this, n);
  }




  void multiply(const Complex& z) {
    double a = this->Re;  double b = this->Im;
    double c = z.Re;      double d = z.Im;
    this->Re = (a*c) - (b*d);
    this->Im = (a*d) + (b*c);
    this->update_PhiRad();
  }

  void operator *=(const Complex& z) {
    this->multiply(z);
  }

  void multiply(const double& n) {
    this->Re *= n;
    this->Im *= n;
    this->update_PhiRad();
  }

  void operator *=(const double& n) {
    this->multiply(n);
  }

  static Complex multiply(const Complex& z1, const Complex& z2) {
    double a = z1.Re;   double b = z1.Im;
    double c = z2.Re;   double d = z2.Im;
    return Complex((a*c) - (b*d), (a*d) + (b*c));
  }

  Complex operator *(const Complex& z) {
    return Complex::multiply(*this, z);
  }

  static Complex multiply(const Complex& z, const double& n) {
    return Complex(z.Re * n, z.Im * n);
  }

  Complex operator *(const double& n) {
    return Complex::multiply(*this, n);
  }


  void divide(const double& n) {
    this->Re /= n;
    this->Im /= n;
    this->update_PhiRad();
  }

  void operator /=(const double& n) {
    return this->divide(n);
  }

  static Complex divide(const Complex& z, const double& n) {
    return Complex(z.Re/ n, z.Im / n);
  }

  Complex operator /(const double& n) {
    return Complex::divide(*this, n);
  }




  
  void scale_ReIm(const double& scale_Re, const double& scale_Im) {
    this->Re *= scale_Re;
    this->Im *= scale_Im;
    this->update_PhiRad();
  }

  static Complex scale_ReIm(const Complex& z, const double& scale_Re, const double& scale_Im) {
    return Complex(z.Re*scale_Re, z.Im*scale_Im);
  }


  void stretch(const double& scale_rad) {
    this->rad *= scale_rad;
    this->update_ReIm(); 
  }

  static Complex stretch(const Complex& z, const double& scale_rad) {
    Complex Z = Complex::clone(z);
    Z.rad *= scale_rad;
    Z.update_ReIm();

    return Z;
  }


  void rotate(const double& angle) {
    this->phi += angle;
    while(this->phi < -PI) this->phi += PI;
    while(this->phi >  PI) this->phi -= PI;
    this->update_ReIm();
  }

  static Complex rotate(const Complex& z, const double& angle) {
    Complex Z = Complex::clone(z);
    Z.phi += angle;
    while(Z.phi < -PI) Z.phi += PI;
    while(Z.phi >  PI) Z.phi -= PI;
    Z.update_ReIm();

    return Z;
  }
  
};
