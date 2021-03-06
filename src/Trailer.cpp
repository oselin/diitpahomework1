
#include "car_trailer.h"

#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>
#include <ostream>

using std::cout;
using std::endl;
using std::string;
using std::to_string;




//Method: distributeOffset() [PUBLIC]
void oselin::Trailer::distributeOffset(){

    this->downfloor.offset(this->offset());
    this->upfloor.offset(this->offset());
    this->front_wheel.offset(this->offset());
    this->rear_wheel.offset(this->offset());
    this->rear_joint.offset(this->offset());
    this->front_joint.offset(this->offset());
    this->rear_axis.offset(this->offset());
    this->front_axis.offset(this->offset());
    this->rear_joint.body.offset(this->offset());
    this->rear_joint.head.offset(this->offset());
    this->front_joint.body.offset(this->offset());
    this->front_joint.head.offset(this->offset());
    this->front_axis.body.offset(this->offset());
    this->front_axis.bottom_screw.offset(this->offset());
    this->front_axis.top_screw.offset(this->offset());
    this->rear_axis.body.offset(this->offset());
    this->rear_axis.bottom_screw.offset(this->offset());
    this->rear_axis.top_screw.offset(this->offset());
    
}

//Method: fromSvg2Param() [PUBLIC]
void oselin::Trailer::fromSvg2Param(){
    this->isempty(0);
    this->distributeOffset();
    this->car_radius(20 * this->rear_wheel.radius()/this->downfloor.height());

    this->x(this->rear_joint.head.x());
    this->y(this->rear_joint.head.y());
    this->trailer_length(this->downfloor.width());
    this->trailer_height(this->downfloor.height());

    this->n_cars((this->trailer_length()-1.5*this->car_length())/(1.5*this->car_length()));
    this->margin(this->car_length()/2);
    
    this->height(this->car_height() * this->n_floors() + 100);
    this->length(this->trailer_length()+ 2*this->rear_joint.length() - 2* this->rear_joint.head.radius());
}

//Method: copy() [PUBLIC]
oselin::Trailer* oselin::Trailer::copy(){

    oselin::Parameters buffer(this);
    buffer.length(this->length()/(this->n_cars() + (this->n_cars()+3)/2));

    buffer.height((this->height() -100)/ (int)this->n_floors());
    oselin::Trailer *acopyof = new oselin::Trailer(buffer, true, false);

    return acopyof;
}

//Method: measures()
string oselin::Trailer::measures() const{
    string measure;
    if (this->n_floors() > 1){
        oselin::Floor line;
        line.color("black");
        line.stroke(0);
        line.strokecolor("");
        line.offset(this->offset());

        
        oselin::Floor limSX = line;
        oselin::Floor limDX = line;

        line.x(this->x()-0.3*this->height());
        line.y(this->y() - this->height() + this->downfloor.height()/2);
        line.width(4);
        line.height(this->height() - this->downfloor.height());

        limSX.x(line.x() - 0.1*this->height());
        limSX.y(line.y());
        limSX.width(0.8*this->height());
        limSX.height(1);

        limDX.x(line.x() - 0.1*this->height());
        limDX.y(line.y() + line.height());
        limDX.width(0.8 *this->height());
        limDX.height(1);

        measure += "\n" + line.svg()
                + "\n" + limSX.svg()
                + "\n" + limDX.svg()
                + oselin::textToSvg(line)
                + this->upfloor.dimensioning();
    
    }else measure += this->downfloor.dimensioning();


    measure += this->rear_wheel.dimensioning()
            +  this->front_joint.dimensioning();

    return measure;
}

//Constructor
oselin::Trailer::Trailer(){
    this->isempty(true);
}

//Copy Constructor by reference
oselin::Trailer::Trailer(Trailer &t): oselin::Parameters(t){
    this->distributeOffset();
}

//Copy Constructor by address
oselin::Trailer::Trailer(Trailer *t): oselin::Parameters(t){
    this->distributeOffset();
}

//Constructor - PARAMETERS
oselin::Trailer::Trailer(oselin::Parameters parameters, bool avoid_svg, bool automatic_offset){
    if (parameters.car_radius() < 16 || parameters.car_radius() > 18){
        throw std::out_of_range("Radius value is wrong!");
    }

    //Estimating the required margin values
    parameters.margin(parameters.car_length()/2);
    
    //Redefining length and height
    parameters.trailer_length(parameters.car_length() * parameters.n_cars()  + (parameters.n_cars()+3)*parameters.car_length()/2);
    parameters.height(parameters.car_height() * parameters.n_floors() + 100);
    
    //CONSTRAINTS
    if (!avoid_svg){
        if (parameters.svg_width() < parameters.trailer_length()){
            throw std::invalid_argument("INIT: WIDTH ERROR");
        }
        if (parameters.svg_height() < parameters.trailer_height()*5/4){
            throw std::invalid_argument("INIT: HEIGHT ERROR");
        }
    }
    if (parameters.n_cars() == 1 && parameters.n_floors() == 2){
        throw std::invalid_argument("INIT: STRUCTURAL ERROR");
    }
    if (parameters.car_length() < (parameters.car_height()*4/5)){
        throw std::invalid_argument("INIT: CAR DIMENSIONS ERROR");
    }
    if (parameters.car_length() > (parameters.car_height()*10)){
        throw std::invalid_argument("INIT: CAR DIMENSIONS ERROR");
    }
    if (parameters.n_cars() > 2){
        throw std::out_of_range("Too many cars!");
    }
    if (parameters.n_floors() > 2){
        throw std::out_of_range("Too many floors!");
    }
    
    this->copyParam(parameters);
    oselin::trigonometry(this, automatic_offset);
    this->distributeOffset();   
}

//Constructor - PARSING
oselin::Trailer::Trailer(string svg, int svg_bypass){

    if (svg != ""){
        int pieces[7][2];
        for (int i=1;i<9;i++){
            int index = svg.find(oselin::checkpoint(i));
            int len = svg.find(oselin::checkpoint(i+1)) - index;
            pieces[i-1][0] = index+11;
            pieces[i-1][1] = len-11;
        }
        try{
            this->offset(stof(oselin::buffering(svg.substr(0,pieces[0][0]), "<!--of", '>')));
            this->car_length(stof(oselin::buffering(svg.substr(0,pieces[0][0]), "<!--cl", '>')));
            this->car_height(stof(oselin::buffering(svg.substr(0,pieces[0][0]), "<!--ch", '>')));
        }catch(std::exception &e){throw std::domain_error("No data to convert.");}

        if (this->offset() == 0 || this->car_length() == 0 || this->car_height() == 0) throw std::logic_error("File seems corrupted.");

        if (!svg_bypass){
            this->svg_width   (stof(oselin::buffering(svg.substr(0,pieces[0][0]),"width='", '\'')));
            this->svg_height  (stof(oselin::buffering(svg.substr(0,pieces[0][0]),"height='",'\'')));
        }
       
        this->rear_joint      = oselin::Joint(svg.substr(pieces[0][0], pieces[0][1]), this->offset());
        this->front_joint     = oselin::Joint(svg.substr(pieces[1][0], pieces[1][1]), this->offset());
        this->downfloor       = oselin::Floor(svg.substr(pieces[2][0], pieces[2][1]), this->offset());
        this->front_wheel     = oselin::Wheel(svg.substr(pieces[3][0], pieces[3][1]), this->offset());
        this->rear_wheel      = oselin::Wheel(svg.substr(pieces[4][0], pieces[4][1]), this->offset());

        if (pieces[5][1] > 0){
            this->upfloor    = oselin::Floor(svg.substr(pieces[5][0], pieces[5][1]), this->offset());
            this->rear_axis  = oselin::Axis (svg.substr(pieces[6][0], pieces[6][1]), this->offset());
            this->front_axis = oselin::Axis (svg.substr(pieces[7][0], pieces[7][1]), this->offset());
            this->n_floors(2);
        }
        else this->n_floors(1);

        
        this->fromSvg2Param();
    }
    else throw std::range_error("File is empty!");
}

//Method: svg() [PUBLIC]
string oselin::Trailer::svg(bool with_header, bool with_measures) const{
    
    string svg;
    if (this->isempty() == 0){
        if (with_header){
            svg += "<?xml version='1.0' encoding='UTF-8' standalone='no'?>\n<svg xmlns='http://www.w3.org/2000/svg' width='";
            svg += to_string(this->svg_width()) + " '  height='";
            svg += to_string(this->svg_height()) + "' >";
            svg += "<rect  x='0.000000' y='0.000000' width='" + to_string(this->svg_width()) + "' height='" + to_string(this->svg_height()) + "' style='stroke-width:0.0; stroke:' fill='white' />";
        }

        //Save some data straight into the file
        svg += "<!--of" + to_string(this->offset()) + "-->\n";
        svg += "<!--cl" + to_string(this->car_length()) + "-->\n";
        svg += "<!--ch" + to_string(this->car_height()) + "-->\n";

        svg += "\n<!--#1-->";
        svg += "\n" + this->rear_joint.svg();
        svg += "\n<!--#2-->";
        svg += "\n" +this->front_joint.svg();
        svg += "\n<!--#3-->";
        svg += "\n" + this->downfloor.svg();
        svg += "\n<!--#4-->";
        svg += "\n" + this->front_wheel.svg();
        svg += "\n<!--#5-->";
        svg += "\n" + this->rear_wheel.svg();
        
        if (this->n_floors() > 1) {
            svg += "\n<!--#6-->";
            svg += "\n" + this->upfloor.svg();
            svg += "\n<!--#7-->";
            svg += "\n" + this->rear_axis.svg();
            svg += "\n<!--#8-->";
            svg += "\n" + this->front_axis.svg();
        }
        
        if (with_measures) svg += this->measures();

            
        return svg;
    } return "";
}

