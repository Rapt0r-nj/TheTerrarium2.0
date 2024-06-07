
// ded blue 0
// live green countdown
// ghost purple 1-cd
// space black -10
// angel white -20
// devil red -66


typedef struct color{
  int R;
  int G;
  int B;
}color;

typedef struct scheme{
  color dd;
  color lv;
  color gh;
  color sp;
  color ng;
  color dv;

}scheme;

scheme const cattpuccin = {{59, 61, 80}, {166, 227, 161}, {203, 166, 247}, {24, 24, 37}, {205, 214, 244}, {243, 139, 168}};
scheme const tokyo      = {{59,66,97}, {195,232,141}, {157,124,216}, {31,35,53}, {244,244,244}, {255, 0, 124}};
scheme const dracula    = {{88 ,104, 154}, {80, 250, 123}, {189 ,147 ,249}, {40 ,42 ,54}, {248 ,248 ,242}, {255, 85, 85}};

scheme themes[] = {
cattpuccin,
tokyo,
dracula
};


