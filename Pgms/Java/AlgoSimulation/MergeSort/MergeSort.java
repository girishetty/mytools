/***********************************************************/
/* MergeSort.java                                          */
/* A MergeSort demonstration algorithm                     */
/* By JungSig Min attending Myong Ji University            */
/***********************************************************/

import java.util.*;
import java.awt.*;
import java.applet.*;
import java.lang.*;

/***********************************************************/
/*   MergeSort class                                       */
/* A MergeSort demonstration algorithm                     */
/* By JungSig Min attending Myong Ji University            */
/***********************************************************/
public class MergeSort extends java.applet.Applet implements Runnable {

  static final int maxArrayLength=16;
  private Font 	font;                         //font variable
  private Panel	panel1,panel2;                //panel variables
  private Button button1,button2,button3,button4; //button variables
  private Label label1,slow,fast;             //label variables
  private TextField textfield;                //textfield variable
  private Scrollbar speed;                    //scrollbar variable
  private int speedFactor=200;                //animation speed
  int high[]=new int[30];                     //higher index 
                                              //of working array
  int low[]=new int[30];                      //lower index 
                                              //of working array
  int level=0;                                //level of the recursion
  int workArray[]=new int[30];                //working array
  int workLength;                             //   and its length 
  int workIdx,arrayIdx;                       //two indice for arrays
  int m1Idx,m2Idx;                            //m1,m2 indice
  int assignCount=0;                          //assignment counter
  int compCount=0;                            //comparison counter 
  boolean moveBar;                            //whether to move bars
  boolean showWork;                           //   and whether to show working array  
  int a[]={ 13,6,4,14,3,12,11,7,15,9,16,5,2,1,8,10 };// initial data
  int b[]={ 13,6,4,14,3,12,11,7,15,9,16,5,2,1,8,10 };// initial data backup
  int arrayLength=maxArrayLength;             //a[] array length 
  Thread runner;                              //mergesort running thread

  // algorithm source code array
  String code [] = { 
    "void mergesort(int a[], int low, int high)  {",
    "  if(low == high) ",
    "       return;",
    "  int length = high-low+1;",
    "  int pivot = (low+high) / 2;",
    "  mergesort(a, low, pivot);",
    "  mergesort(a, pivot+1, high);",
    "  int working[] = new int[length];",
    "  for(int i = 0; i < length; i++) ",
    "      working[i] = a[low+i];",
    "  int m1 = 0; ",
    "  int m2 = pivot-low+1;",
    "  for(int i = 0; i < length; i++) {",
    "    if(m2 <= high-low) ",
    "        if(m1 <= pivot-low) ",
    "            if(working[m1] > working[m2]) ",
    "                a[i+low] = working[m2++];  ",
    "            else ",
    "                a[i+low] = working[m1++];",
    "        else ",
    "            a[i+low] = working[m2++];",
    "    else ",
    "        a[i+low] = working[m1++];",
    "  }",
    "}"
    };

  // algorithm pseudo code array
  String pseudoCode [] = { 
    "Enter mergesort with the input array, high index and low index",
    "If low index equals high index ",
    "Return to caller ",
    "Calculate the length ",
    "Calculate the pivot",
    "Mergesort the half that is before pivot",
    "Mergesort the half that is after pivot",
    "Allocate space for a temporary working array",
    "A for loop to copy input array to working array",
    "A for loop to copy input array to working array",
    "Assign local variable m1 as index of first half",
    "Assign local variable m2 as index of second half",
    "A for loop ends until i reaches length",
    "If m2 is less than or equal to high-low",
    "If m1 is less than or equal to pivot-low",
    "If working[m1] is greater than working[m2]",
    "Assign working[m2] to a[i+low]",
    "Else if working[m1] is greater than working[m2]",
    "Assign working[m1] to a[i+low]",
    "Else if  m1 is greater than pivot-low",
    "Assign working[m2] to a[i+low]",
    "Else if m2 is larger than high-low",
    "Assign working[m1] to a[i+low]",
    "Continue of the for loop",
    "End of MergeSort"
  };

  CodePanel codeDisplay;                         //declare codepanel
  AnimationCanvas animationCanvas;               //declare animationcanvas
  int granularity = 0;                           //declare granularity
  Choice granularityChoice = new Choice ();      //declare granularity choice
  //declare granularity labels
  String granularityLabels [] = {"Entire Sort", "Next Merge", "Next Line"};

  // initialize the mergesort 
  public void init() {
    
    high[0]=-1;                              //initialize work range indice
    low[0]=-1;
    level=0;
    
    setLayout(new BorderLayout());            //set the layout of the applet
	
    panel1 = new Panel();                     //new panel for buttons,etc.
    panel1.setLayout(new FlowLayout());       //set panel layout
    panel1.add(button1=new Button("Reset"));  //add buttons to the panel
    panel1.add(button2=new Button("Sort"));
    panel1.add(button3=new Button("Step"));
    //add items to the granularity choice
    for (int i = 0; i < granularityLabels.length; i++) {
      granularityChoice.addItem (granularityLabels [i]);
    }
    panel1.add (granularityChoice);           //add granularity choice to panel
    panel1.add( slow = new Label ( "Slow" ) );//add slow label to panel
    //add speed scrollbar to panel
    panel1.add( speed = new Scrollbar( Scrollbar.HORIZONTAL, 5, 1, 1, 10 ) );
    panel1.add( fast = new Label ( "Fast" ) );//add fast label to panel
	
    panel2 = new Panel();                     //new panel for text field
    panel2.add(label1=new Label("Unsorted Array:"));//add label to panel
    //add textfield to the panel
    panel2.add(textfield=new TextField("13,6,4,14,3,12,11,7,15,9,16,5,2,1,8,10,",45));
    panel2.add(button4 = new Button("Accept")); // add button to panel2
    add("North",panel2);                      //add panel2 to north of applet
    add("South",panel1);                      //add panel1 to south of applet 
    animationCanvas=new AnimationCanvas(this);//new animationcanvas
    add ("Center", animationCanvas);          //add animationcanvas to the center
    codeDisplay = new CodePanel (code, pseudoCode, this); //new code display
    add ("East", codeDisplay);                //add codedisplay to the east 
  }
  
  // thread runner start
  public void startsort() {
    if (runner == null); {                   //if runner thread doesn't exist
      runner = new Thread(this);
      runner.start();
    }
  }

  // thread runner stop
  public void stopsort() {
    if (runner != null) {                    //if runner thread does exist
      runner.stop();
      runner = null;
    }
  }
  
  //run the mergesort here
  public void run() {
    mergesort(a,0,arrayLength-1);// Sort button is clicked
    //		animationCanvas.repaint();
    
  }

  //action handler here 
  public boolean action(Event evt, Object arg) {
    if (arg.equals("Sort")) {      // Sort button is clicked
      high[0]=-1;                  //initialize work range indice
      low[0]=-1;
      level=0;
      button2.setLabel("Stop");    //change the button label
      workLength=arrayLength;      //initialize working array length
      System.arraycopy(b, 0, a, 0, b.length);  // copy from b[] to a[]
      System.arraycopy(a, 0,workArray,0,workLength); // copy from a[] to wrok array
      
      startsort();                 //start sorting
    }
    else if (arg.equals("Accept")) { // Accept button is clicked
      high[0]=-1;                  //initialize work range indice
      low[0]=-1;
      level=0;
      stopsort();                  //stop sorting
      button2.setLabel("Sort");    //change the button label
      resetarray();                //accept the input array from textfield
      animationCanvas.repaint();   //repaint the animation canvas
    }
    
    else if (arg.equals("Reset")) { // Reset button is clicked
      high[0]=-1;                  //initialize work range indice
      low[0]=-1;
      level=0;
      workLength=0;                //initialize working array length
      moveBar=false;               //disable bar movement
      assignCount=0;               //initialize assignment counter               
      compCount=0;                 //   and comparison counter
      stopsort();                  //stop sorting
      button2.setLabel("Sort");    //change the button label
      System.arraycopy(b, 0, a, 0, b.length);  // copy from b[] to a[]
      animationCanvas.repaint();   //repaint the animation canvas
    }
    else if (arg.equals("Step")) {  // Step button is clicked
      if (runner!=null)                    //if runner thread does exist
	runner.resume();               // resume the runner thread
    }
    else if (arg.equals("Stop")) {  // Stop button is clicked
      button2.setLabel("Sort");    //change the button label
      stopsort();                  //stop sorting
      moveBar=false;               //disable bar movement
      
    }
    else if (evt.target == granularityChoice) { //if Granuality is chosen
      granularity = granularityChoice.getSelectedIndex ();  //assign the chosen value
    }
    else if ( evt.target == speed ) {  //if Speed scroll bar is chosen
      speedFactor=1600 *(11- speed.getValue())/10;//assign the adjusted value
    }
    
    else return super.action(evt,arg);   // other events happened
    return true;
  }
  
  //reset the array of a[] according to the textfield
  void resetarray() {
    int i,j,temp;                   //local variables used in this procedure
    String input,subinput;
    int beginIndex,endIndex;
    input=textfield.getText();      //get the textfield content

    //parsing analysis of input contents and store the input into the a[] and b[]
    i=0;
    j=0;
    while (i< input.length() && j<maxArrayLength) {
      while (i< input.length() && (input.charAt(i)>'9' || input.charAt(i)<'0'))
	i++;
      if(i == input.length() )
	break;
      temp=0;
      while (i< input.length() && input.charAt(i)<='9' && input.charAt(i)>='0') {
	temp=temp*10+(int)input.charAt(i)-(int)'0';
	i++;
      }
      if(i == input.length() )
	break;
      
      a[j]=temp;
      b[j]=a[j];
      j++;
    }
    arrayLength=j;            //indicate the input array length
  }   	
  

  //mergesort procedure
  void mergesort(int a[], int lo, int hi)  {
    showWork=false;                 //disable showing the work array
    level++;                        //increase the level of recursion
    high[level]=hi;                 //assign the work range indice
    low[level]=lo;
    animationCanvas.repaint();    //repaint the animation canvas
    try { Thread.sleep(speedFactor); }  //thread sleep for a while
    catch (InterruptedException e) { }
    // Base case
    visualize (0, 1);              //highlight the currently executed line
    visualize (1, 2);
    if(lo == hi) {                 //if low and high indice of the array equal to each other
      compCount++;                 //increase comparison counter
      visualize (2, 2);              //highlight the currently executed line
      high[level]=hi;                //assign the work range indice
      low[level]=lo;
      level--;                        //decrease the level of recursion
      animationCanvas.repaint();    //repaint the animation canvas
      try { Thread.sleep(speedFactor); }  //thread sleep for a while
      catch (InterruptedException e) { }
      return;
    }
    
    // Recurse
    visualize (3, 2);              //highlight the currently executed line
    int length = hi-lo+1;          //calculate the length of the currenly sorting part
    assignCount++;                 //increase assignment counter
    visualize (4, 2);              //highlight the currently executed line
    int pivot = (lo+hi) / 2;
    assignCount++;                 //increase assignment counter
    visualize (5, 2);              //highlight the currently executed line
    mergesort(a, lo, pivot);
    visualize (6, 2);              //highlight the currently executed line
    mergesort(a, pivot+1, hi);
    // Merge
    visualize (7, 2);              //highlight the currently executed line
    int working [] = new int[length];
    visualize (8, 2);              //highlight the currently executed line
    for(int i = 0; i < length; i++) { 
      working[i] = a[lo+i];
      workArray[i]=a[lo+i];
    }
    workLength = length;
    assignCount+=length;                 //increase assignment counter
    
    visualize (10, 2);              //highlight the currently executed line
    int m1 = 0;
    assignCount++;                 //increase assignment counter
    m1Idx=m1;
    visualize (11, 2);              //highlight the currently executed line
    int m2 = pivot-lo+1;
    assignCount++;                 //increase assignment counter
    m2Idx=m2;
    showWork=true;
    high[level]=hi;
    low[level]=lo;
    animationCanvas.repaint();    //repaint the animation canvas
    try { Thread.sleep(speedFactor/2); }  //thread sleep for a while
    catch (InterruptedException e) { }
    visualize (12, 2);              //highlight the currently executed line
    for(int i = 0; i < length; i++) {
      visualize (13, 2);              //highlight the currently executed line
      if(m2 <= hi-lo) {
    	compCount++;                 //increase comparison counter
	visualize (14, 2);              //highlight the currently executed line
        if(m1 <= pivot-lo) {
	  compCount++;                 //increase comparison counter
	  visualize (15, 2);              //highlight the currently executed line
          if(working[m1] > working[m2]) {
	    compCount++;                 //increase comparison counter
	    visualize (16, 2);              //highlight the currently executed line
	    workIdx=m2++;
	    m2Idx=m2;
            //a[i+lo] = working[m2++];
	    assignCount++;                 //increase assignment counter
	  }
          else {
	    compCount++;                 //increase comparison counter
	    visualize (17, 2);              //highlight the currently executed line
	    visualize (18, 2);
	    workIdx=m1++;
	    m1Idx=m1;
            //a[i+lo] = working[m1++];
	    assignCount++;                 //increase assignment counter
          }
        }
        else {
    	compCount++;                 //increase comparison counter
	visualize (19, 2);              //highlight the currently executed line
	visualize (20, 2);
	workIdx=m2++;
	m2Idx=m2;
	//a[i+lo] = working[m2++];
 	assignCount++;                 //increase assignment counter
        }
      }
      else {
    	compCount++;                 //increase comparison counter
	visualize (21, 2);              //highlight the currently executed line
	visualize (22, 2);
	workIdx=m1++;
	m1Idx=m1;
	//a[i+lo] = working[m1++];
 	assignCount++;                 //increase assignment counter
      }
      arrayIdx=i;
      visualize (23, 2);              //highlight the currently executed line
      
      moveBar=true;
      
      animationCanvas.move(workIdx,arrayIdx);
      visualize (12, 2);              //highlight the currently executed line
      moveBar=false;
      a[i+lo]=working[workIdx];
      animationCanvas.repaint();    //repaint the animation canvas
      try { Thread.sleep(speedFactor/2); }  //thread sleep for a while
      catch (InterruptedException e) { }
    }
    level--;
    animationCanvas.repaint();    //repaint the animation canvas
    try { Thread.sleep(speedFactor); }  //thread sleep for a while
    catch (InterruptedException e) { }
    visualize (24, 2);              //highlight the currently executed line
    showWork=false;
  }
  
  //handle WINDOW_DESTROY event here 
  public boolean handleEvent(Event evt) {
    if (evt.id == Event.WINDOW_DESTROY) 
      System.exit(0);
    else return super.handleEvent(evt);
    return true;
  }
  
  //visualize the code panel
  void visualize (int line, int level) {
    codeDisplay.highlightLine (line);          //highlight the currently executed line
    if (level > granularity) {                 //if current level is larger than granularity
      try {Thread.sleep (speedFactor);}   //thread sleep for a while
      catch (Exception e) {};
    } 
    else {
      runner.suspend ();                 //suspend the runner thread
    }
  }
  
}


/***********************************************************/
/*   AnimationCanvas class                                 */
/* A MergeSort demonstration algorithm                     */
/* By JungSig Min attending Myong Ji University            */
/***********************************************************/

class AnimationCanvas extends Canvas {
 
  static final int dBarWidth=15;                //constant variables for the positions
  static final int dBarHeight=6;                //    of the animation canvas
  static final int dBarGap=5;
  static final int dBarSpace=dBarWidth+dBarGap;
  static final int originX1=10;
  static final int originY1=120;
  
  static final int originX2=10;
  static final int originY2=264;
 
  static final int originX3=10;
  static final int originY3=295;
  Color skyBlue=new Color(116,224,255);        //different color definations
  Color sortedColor=new Color(255,159,77);
  Color unsortedColor=new Color(0,91,131);
  Color rocketColor=new Color(255,122,100);
  
  MergeSort applet;                            //declare mergesort applet
  Image offscreenImage;                        //offscreenImage for double buffering
  Graphics offscreenGraphics;                  //offscreenGraphics for double buffering
  int moveX,moveY,moveWidth,moveHeight;        //movement parameters
  boolean showFlame;                           //whether to show fire flames of rocket
  

  //constructor of AnimationCanvas
  public AnimationCanvas (  MergeSort applet ) {
    this.applet = applet;                      //introduce MergeSort applet
    setBackground (skyBlue);                   //setBackground color
    
  }
  
  public void init() {
    //    offscreenImage= createImage(this.size().width,this.size().height);
    //    offscreenGraphics=offscreenImage.getGraphics();
  }
  
  //get prefered size
  public Dimension preferredSize () {
    return new Dimension (200, 100);
  }
  
  //get minimum size
  public Dimension minimumSize () {
    return preferredSize ();
  }
  
  //bar movement like a rocket
  public void move(int workIdx, int arrayIdx ){
    int i;	                           //some local variables
    int value=applet.workArray[workIdx];
    
    int X1=originX1+(applet.low[applet.level]+workIdx)*dBarSpace;
    int X2=originX2+(applet.low[applet.level]+arrayIdx)*dBarSpace;
    int Y1=originY2-value*dBarHeight;
    int Y2=originY1-value*dBarHeight;
    moveY=Y1;
    moveWidth=dBarWidth;
    moveHeight=value*dBarHeight;
    showFlame=false;                      //don't shower fire flame when move horizontally
    if (X1<X2)                            
      for ( moveX=X1; moveX<=X2;moveX+=5) { //move horizontally from left to right
	repaint();                       //repaint the animation canvas
      }
    else if (X1>X2)
      for ( moveX=X1; moveX>=X2;moveX-=5) {//move horizontally from right to left
	repaint();                      //repaint the animation canvas
      }
    
    moveX=X2;
    i=0;
    for (moveY=Y1; moveY>=Y2;moveY-=8) {//move vertically
      if (i%2==0) 
        showFlame=true;                 //show flame then not show flame to make it flashing
      else
        showFlame=false;
      i++;
      repaint();                      //repaint the animation canvas
    }
  }
  

  //paint function on the Graphics
  public void paint(Graphics g) {
    char temp[];	                     //some local variables
    int value;
    int pivot;
    int workLength;
    Polygon poly;

    //text strings on the canvas
    g.drawString("Sorting Array a[ ]",originX1+120,originY1+35);
    g.drawString("Temp Array working[ ]",originX2+100,originY2+25);
    g.drawString("Working Range at Levels of Recursion",originX1+60,originY3+75);
    g.drawString("Comparison Counter = "+applet.compCount+
                 "  Assignment Counter = "+applet.assignCount,originX1,originY3+100);
    //animation for Working Range at Levels of Recursion
    for (int i=1;i<=applet.level;i++) {
	g.setColor(Color.black);
	g.drawString(""+i,2,originY3+i*10);
	g.setColor(Color.gray);
	workLength=applet.high[i]-applet.low[i]+1;
	if (workLength>0)
	  g.fillRect(originX3+applet.low[i]*dBarSpace,originY3+i*10-5, workLength*dBarSpace,6 );
    }  

    //animation for Sorting Array a[ ]
    for (int i=0;i<applet.arrayLength;i++) {
      value=applet.a[i];
      if (i>=applet.low[applet.level] && i<=applet.high[applet.level]) 
	g.setColor(sortedColor);
      else
	g.setColor(unsortedColor);
      g.drawString(""+value,originX1+i*dBarSpace,originY1+20);
      g.fillRect(originX1+i*dBarSpace,originY1-value*dBarHeight,dBarWidth,value*dBarHeight);
    }

    //animation for Temp Array working[ ]
    if (applet.showWork){
      pivot=applet.workLength/2;
      
      for (int i=0;i<applet.workLength;i++) {
	value=applet.workArray[i];
	if (i<applet.m1Idx)
	  g.setColor(Color.blue);
	else if (i< pivot)
	  g.setColor(Color.yellow);
	else if (i< applet.m2Idx)
	  g.setColor(Color.blue);
	else 
	  g.setColor(Color.orange);
	g.fillRect(originX2+(i+applet.low[applet.level])*dBarSpace,originY2-value*dBarHeight,dBarWidth,value*dBarHeight);
	if (i==applet.m1Idx)
	  g.drawString("m1="+i,originX2+(i+applet.low[applet.level])*dBarSpace,originY2+10);
	if (i==applet.m2Idx)
	  g.drawString("m2="+i,originX2+(i+applet.low[applet.level])*dBarSpace,originY2+18);
	
      }
    }  

    //animation for bar movement like a rocket
    if (applet.moveBar) {
      g.setColor(rocketColor);
      int Xs[]= {moveX,moveX+moveWidth/2,moveX+moveWidth,moveX+moveWidth,moveX};
      int Ys[]= {moveY,moveY-moveWidth,moveY,moveY+moveHeight,moveY+moveHeight};
      int pts=Xs.length;
      poly=new Polygon(Xs,Ys,pts);
      g.fillPolygon(poly);
      if (showFlame) {
	for (int i=moveX;i<=moveX+moveWidth;i++){
	  if (i%3==0) 
	    g.setColor(Color.yellow);
	  else if (i%2==1) 
	    g.setColor(Color.red);
	  else
	    g.setColor(Color.white);
	  
	  g.drawLine(i,moveY+moveHeight+5,i,moveY+moveHeight+20);
	}
      }
      
    }
  }
  
  //override repaint() to support double buffering
  public void repaint()
  {
    Image image = this.createImage(size().width, size().height);
    Graphics g = image.getGraphics();
    g.setColor(this.getBackground());
    g.fillRect(0,0,size().width,size().height);
    g.setColor(this.getForeground());
    paint(g);
    this.getGraphics().drawImage(image,0,0,null);
  }
}


/***********************************************************/
/*   CodePanel class                                       */
/* A MergeSort demonstration algorithm                     */
/* By JungSig Min attending Myong Ji University            */
/***********************************************************/

class CodePanel extends Panel {
  
  static final int marginX = 20;                 //constant variables for the positions
  static final int marginY = 10;                 //  for code panel
  static final int offsetX = 1;
  static final int offsetY = 1;
  static final int none    = -1;
  String code [];
  String explanations [];
  Font font = new Font ("TimesRoman", Font.PLAIN, 14);
  int lineHeight;
  int baseLine;
  int maxWidth = 0;
  int highlightedLine = none;
  Label explanation = new Label ();
  Applet applet;
  Color backgroundColor= new Color(24,255,208);
  Color explainColor = Color.gray;

  //constructor for  CodePanel
  public CodePanel (String code [], String explanations [], Applet applet) {
    this.code = code;
    this.explanations = explanations;
    this.applet = applet;
    setBackground (backgroundColor);
    explanation.setBackground (explainColor);
    add (explanation);
    explanation.hide ();
  }
  
  //preferredSize
  public Dimension preferredSize () {
    return new Dimension (maxWidth + 2 * marginX
			  , code.length * lineHeight + 2 * marginY);
  }
  
  //addNotify
  public void addNotify () {
    super.addNotify ();
    setFont (font);
    FontMetrics metrics = getFontMetrics (font);
    baseLine = metrics.getAscent ();
    lineHeight = baseLine + metrics.getDescent ();
    for (int i = 0; i < code.length; i++) {
      maxWidth = Math.max (maxWidth, metrics.stringWidth (code [i]));
    }
    maxWidth+=40;
  }
  
  //reset
  public void reset () {
    if (highlightedLine != none) {
      colorLine (highlightedLine, backgroundColor);
    }
    highlightedLine = none;
  }
  
  //highlight the currently executed line
  public void highlightLine (int line) {
    if (highlightedLine != none) {
      colorLine (highlightedLine, backgroundColor);
    }
    highlightedLine = line;
    if (highlightedLine != none) {
      colorLine (highlightedLine, Color.orange);
    }
  }
  
  //make color rectangle
  public void colorLine (int line, Color color) {
    Graphics g = getGraphics ();
    int y = marginY + line * lineHeight;
    g.setColor (color);
    g.fillRect (0, y, size ().width - 1, lineHeight);
    g.setColor (Color.black);
    g.drawString (code [line], marginX, y + baseLine);
  }
  
  //paint the code panel
  public void paint (Graphics g) {
    int y = marginY + baseLine;
    for (int i = 0; i < code.length; i++, y += lineHeight) {
      g.drawString (code [i], marginX, y);
    }
    highlightLine (highlightedLine);
  }
  
  //if there's mouseExit event
  public boolean mouseExit (Event event, int x, int y) {
    explanation.hide ();
    validate ();
    return true;
  }
  
  //if there's mouseUp event
  public boolean mouseUp (Event event, int x, int y) {
    int line = (y - marginY) / lineHeight;
    if ((line <= explanations.length)
	|| (explanations [line].equals (""))) {
      explanation.setText (explanations [line]);
      explanation.setBackground (explainColor);
      explanation.setForeground (Color.black);
      explanation.validate ();
      explanation.show ();
    } 
    else {
      explanation.hide ();
    }
    validate ();
    explanation.move (marginX + offsetX
		      , marginY + offsetY + (line + 1) * lineHeight);
    return true;
  }
  
}






