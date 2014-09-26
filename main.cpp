#include <flext.h>
#include "Leap.h"
#include "LeapMath.h"
#include "Dispatcher.h"

// check for appropriate flext version
#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 400)
#error You need at least flext version 0.4.0
#endif


class leapmotion: public flext_base
{
    FLEXT_HEADER(leapmotion,flext_base)
    
    // Create a sample listener and controller
    Dispatcher dispatcher;
    Controller controller;
    
    int general_flag;
    
    int hands_sphere_radius_flag;
    int hands_sphere_center_flag;
    
    int hands_direction_flag;
    int hands_palm_normal_flag;
    int hands_palm_position_flag;
    int hands_palm_velocity_flag;
    
    int hands_tool_count_flag;
    int hands_finger_count_flag;

    int hands_basis_flag;
    int hands_confidence_flag;
    int hands_is_right_flag;
    int hands_grab_strength_flag;
    int hands_pinch_strength_flag;
    int hands_arm_flag;
    
    int tools_direction_flag;
    int tools_position_flag;
    int tools_velocity_flag;
    int tools_size_flag;
    
    int fingers_direction_flag;
    int fingers_position_flag;
    int fingers_velocity_flag;
    int fingers_size_flag;
    int gestures_flag;


    const t_symbol *sym_direction,*sym_position,*sym_velocity,*sym_size,*sym_duration,*sym_id;
    const t_symbol *sym_tool,*sym_tool_count;
    const t_symbol *sym_hand,*sym_palm_position,*sym_palm_velocity,*sym_palm_normal,*sym_sphere_radius,*sym_sphere_center;
    const t_symbol *sym_fingers,*sym_finger_count;
    const t_symbol *sym_type,*sym_TYPE_INVALID,*sym_TYPE_SWIPE,*sym_TYPE_CIRCLE,*sym_TYPE_SCREEN_TAP,*sym_TYPE_KEY_TAP;
    const t_symbol *sym_state,*sym_STATE_INVALID,*sym_STATE_START,*sym_STATE_UPDATE,*sym_STATE_STOP;

    //v2
    const t_symbol *sym_basis,*sym_confidence,*sym_is_right,*sym_pinch_strength,*sym_grab_strength,*sym_arm;

    
public:
    leapmotion()
    {
        AddInAnything(); 
        AddOutList();  // general info
        AddOutList();  // hands, fingers, tools
        AddOutList();  // gesture info 

        // register methods
        FLEXT_ADDMETHOD(0,m_bang);
        FLEXT_ADDMETHOD_(0, "general", m_general);
        FLEXT_ADDMETHOD_(0, "hands_direction", m_hands_direction);
        FLEXT_ADDMETHOD_(0, "hands_palm_normal", m_hands_palm_normal);
        FLEXT_ADDMETHOD_(0, "hands_palm_position", m_hands_palm_position);
        FLEXT_ADDMETHOD_(0, "hands_palm_velocity", m_hands_palm_velocity);

        FLEXT_ADDMETHOD_(0, "hands_sphere_center", m_hands_sphere_center);
        FLEXT_ADDMETHOD_(0, "hands_sphere_radius", m_hands_sphere_radius);
        
        FLEXT_ADDMETHOD_(0, "hands_finger_count", m_hands_finger_count);
        FLEXT_ADDMETHOD_(0, "hands_tool_count", m_hands_tool_count);

        FLEXT_ADDMETHOD_(0, "hands_basis", m_hands_basis);
        FLEXT_ADDMETHOD_(0, "hands_confidence", m_hands_confidence);
        FLEXT_ADDMETHOD_(0, "hands_is_right", m_hands_is_right);
        FLEXT_ADDMETHOD_(0, "hands_grab_strength", m_hands_grab_strength);
        FLEXT_ADDMETHOD_(0, "hands_pinch_strength", m_hands_pinch_strength);
        FLEXT_ADDMETHOD_(0, "hands_arm", m_hands_arm);


        FLEXT_ADDMETHOD_(0, "tools_direction", m_tools_direction);
        FLEXT_ADDMETHOD_(0, "tools_position", m_tools_position);
        FLEXT_ADDMETHOD_(0, "tools_velocity", m_tools_velocity);
        FLEXT_ADDMETHOD_(0, "tools_size", m_tools_size);

        FLEXT_ADDMETHOD_(0, "fingers_direction", m_fingers_direction);
        FLEXT_ADDMETHOD_(0, "fingers_position", m_fingers_position);
        FLEXT_ADDMETHOD_(0, "fingers_velocity", m_fingers_velocity);
        FLEXT_ADDMETHOD_(0, "fingers_size", m_fingers_size);
        FLEXT_ADDMETHOD_(0, "gestures", m_gestures);
        FLEXT_ADDMETHOD_(0, "info", m_info);

        // Have the sample listener receive events from the controller
        controller.addListener(dispatcher);
        general_flag = true;
        
        hands_direction_flag = false;
        hands_palm_position_flag = true;
        hands_palm_velocity_flag = false;
        hands_palm_normal_flag = false;
        
        hands_sphere_center_flag = false;
        hands_sphere_radius_flag = false;
        
        hands_finger_count_flag = false;
        hands_tool_count_flag = false;
    
	hands_basis_flag = false;
	hands_confidence_flag = false;
	hands_is_right_flag = false;
	hands_grab_strength_flag = false;
	hands_pinch_strength_flag = false;
	hands_arm_flag = false;

        tools_direction_flag = false;
        tools_position_flag = false;
        tools_velocity_flag = false;
        tools_size_flag = false;

        fingers_direction_flag = false;
        fingers_position_flag = false;
        fingers_velocity_flag = false;
        fingers_size_flag = false;
        gestures_flag = false;

	// Initialize symbols
	//
	sym_direction = MakeSymbol("direction");
	sym_position = MakeSymbol("position");
	sym_velocity = MakeSymbol("velocity");
	sym_size = MakeSymbol("size");
	sym_duration = MakeSymbol("duration");
	sym_id = MakeSymbol("id");
	//
	sym_tool = MakeSymbol("tool");
	sym_tool_count = MakeSymbol("tool_count");
	//
	sym_hand = MakeSymbol("hand");
	sym_palm_position = MakeSymbol("palm_position");
	sym_palm_velocity = MakeSymbol("palm_velocity");
	sym_palm_normal = MakeSymbol("palm_normal");
	sym_sphere_radius = MakeSymbol("sphere_radius");
	sym_sphere_center = MakeSymbol("sphere_center");
	//
	sym_fingers = MakeSymbol("fingers");	
	sym_finger_count = MakeSymbol("finger_count");
	//
	sym_type = MakeSymbol("type");
	sym_TYPE_INVALID = MakeSymbol("TYPE_INVALID");
	sym_TYPE_SWIPE = MakeSymbol("TYPE_SWIPE");
	sym_TYPE_CIRCLE = MakeSymbol("TYPE_CIRCLE");
	sym_TYPE_SCREEN_TAP = MakeSymbol("TYPE_SCREEN_TAP");
	sym_TYPE_KEY_TAP = MakeSymbol("TYPE_KEY_TAP");
	//
	sym_state = MakeSymbol("state");
	sym_STATE_INVALID = MakeSymbol("STATE_INVALID");
	sym_STATE_START = MakeSymbol("STATE_START");
	sym_STATE_UPDATE = MakeSymbol("STATE_UPDATE");
	sym_STATE_STOP = MakeSymbol("STATE_STOP");

	//v2
	sym_basis = MakeSymbol("basis");
	sym_confidence = MakeSymbol("confidence");
	sym_is_right = MakeSymbol("is_right");
	sym_grab_strength = MakeSymbol("grab_strength");
	sym_pinch_strength = MakeSymbol("pinch_strength");
	sym_arm = MakeSymbol("arm");

    }
    ~leapmotion()
    {
        // Remove the sample listener when done
        controller.removeListener(dispatcher);
    }
  
    void m_bang()
    {

        Frame frame;
        AtomList generalInfo(6);
        int num_hands, num_tools, num_gestures;
        frame = dispatcher.frame;
        num_hands = frame.hands().count();
        num_tools = frame.tools().count();
        num_gestures = frame.gestures().count();
        
        if(general_flag){
            SetFloat(generalInfo[0], (float)frame.id());
            SetFloat(generalInfo[1], (float)frame.timestamp());
            SetFloat(generalInfo[2], (float)num_hands);
            SetFloat(generalInfo[3], (float)frame.fingers().count());
            SetFloat(generalInfo[4], (float)frame.tools().count());
            SetFloat(generalInfo[5], (float)frame.gestures().count());
            ToOutList(0, generalInfo);        
        }
        // tools
	// TO UPGRADE
        for(int i = 0; i<num_tools; i++){
            Tool tool;
            tool = frame.tools()[i];
	    
            AtomList toolInfo(5);
            if(tools_position_flag) {
                SetFloat(toolInfo[0], i);
                SetSymbol(toolInfo[1], sym_direction);
                SetFloat(toolInfo[2], tool.direction().x);
                SetFloat(toolInfo[3], tool.direction().y);
                SetFloat(toolInfo[4], tool.direction().z);
                ToOutAnything(1, sym_tool, toolInfo.Count(), toolInfo.Atoms());
            }
            if(tools_position_flag) {
                SetFloat(toolInfo[0], i);
                SetSymbol(toolInfo[1], sym_position);
                SetFloat(toolInfo[2], tool.tipPosition().x);
                SetFloat(toolInfo[3], tool.tipPosition().y);
                SetFloat(toolInfo[4], tool.tipPosition().z);
                ToOutAnything(1, sym_tool, toolInfo.Count(), toolInfo.Atoms());
            }
            if(tools_velocity_flag){
                SetFloat(toolInfo[0], i);
                SetSymbol(toolInfo[1], sym_velocity);
                SetFloat(toolInfo[2], tool.tipVelocity().x);
                SetFloat(toolInfo[3], tool.tipVelocity().y);
                SetFloat(toolInfo[4], tool.tipVelocity().z);
                ToOutAnything(1, sym_tool, toolInfo.Count(), toolInfo.Atoms());
            }
            if(tools_size_flag){
                SetFloat(toolInfo[0], i); 
                SetSymbol(toolInfo[1], sym_size);
                SetFloat(toolInfo[2], tool.width());
                SetFloat(toolInfo[3], tool.length());
                ToOutAnything(1, sym_tool, 4, toolInfo.Atoms()); // hmm..
	    }
        }
        // hands and fingers
	for(int i = 0; i<num_hands; i++){
            Hand hand;
            hand = frame.hands()[i];
            int num_fingers = hand.fingers().count();
            int num_tools = hand.tools().count();
            AtomList handInfo(5);


            if(hands_direction_flag){
                // direction
                SetFloat(handInfo[0], i);
                SetSymbol(handInfo[1], sym_direction);
                SetFloat(handInfo[2], hand.direction().x);
                SetFloat(handInfo[3], hand.direction().y);
                SetFloat(handInfo[4], hand.direction().z);
                ToOutAnything(1, sym_hand, handInfo.Count(), handInfo.Atoms());
            }
            if(hands_palm_position_flag){
                // position
                SetFloat(handInfo[0], i);
                SetSymbol(handInfo[1], sym_palm_position);
                SetFloat(handInfo[2], hand.palmPosition().x);
                SetFloat(handInfo[3], hand.palmPosition().y);
                SetFloat(handInfo[4], hand.palmPosition().z);
                ToOutAnything(1, sym_hand, handInfo.Count(), handInfo.Atoms());
            }
            if(hands_palm_velocity_flag){
                // velocity
                SetFloat(handInfo[0], i);
                SetSymbol(handInfo[1], sym_palm_velocity);
                SetFloat(handInfo[2], hand.palmVelocity().x);
                SetFloat(handInfo[3], hand.palmVelocity().y);
                SetFloat(handInfo[4], hand.palmVelocity().z);
                ToOutAnything(1, sym_hand, handInfo.Count(), handInfo.Atoms());
            }
            if(hands_palm_normal_flag){
                // normal
                SetFloat(handInfo[0], i);
                SetSymbol(handInfo[1], sym_palm_normal);
                SetFloat(handInfo[2], hand.palmVelocity().x);
                SetFloat(handInfo[3], hand.palmVelocity().y);
                SetFloat(handInfo[4], hand.palmVelocity().z);
                ToOutAnything(1, sym_hand, handInfo.Count(), handInfo.Atoms());
            }
            if(hands_sphere_radius_flag){
                // sphere radius
                SetFloat(handInfo[0], i);
                SetSymbol(handInfo[1], sym_sphere_radius);
                SetFloat(handInfo[2], hand.sphereRadius());
                ToOutAnything(1, sym_hand, 3, handInfo.Atoms());
            }
            if(hands_sphere_center_flag){
                // sphere center
                SetFloat(handInfo[0], i);
                SetSymbol(handInfo[1], sym_sphere_center);
                SetFloat(handInfo[2], hand.sphereCenter().x);
                SetFloat(handInfo[3], hand.sphereCenter().y);
                SetFloat(handInfo[4], hand.sphereCenter().z);
                ToOutAnything(1, sym_hand, handInfo.Count(), handInfo.Atoms());
            }
            if(hands_finger_count_flag){
                // finger count
                SetFloat(handInfo[0], i);
                SetSymbol(handInfo[1], sym_finger_count);
                SetFloat(handInfo[2], num_fingers);
                ToOutAnything(1, sym_hand, 3, handInfo.Atoms());
            }
            if(hands_tool_count_flag){
                // tool count
                SetFloat(handInfo[0], i);
                SetSymbol(handInfo[1], sym_tool_count);
                SetFloat(handInfo[2], num_tools);
                ToOutAnything(1, sym_hand, 3, handInfo.Atoms());
            }


	    // v2
	    if(hands_basis_flag){
		AtomList handBasisInfo(11);
		Matrix handBasis = hand.basis(); 

		SetFloat(handBasisInfo[0], i);
		SetSymbol(handBasisInfo[1], sym_basis);
		SetFloat(handBasisInfo[2], handBasis.xBasis.x); 
		SetFloat(handBasisInfo[3], handBasis.xBasis.y); 
		SetFloat(handBasisInfo[4], handBasis.xBasis.z); 
		SetFloat(handBasisInfo[5], handBasis.yBasis.x); 
		SetFloat(handBasisInfo[6], handBasis.yBasis.y); 
		SetFloat(handBasisInfo[7], handBasis.yBasis.z);
		SetFloat(handBasisInfo[8], handBasis.zBasis.x);  
		SetFloat(handBasisInfo[9], handBasis.zBasis.y);  
		SetFloat(handBasisInfo[10], handBasis.zBasis.z);  
		ToOutAnything(1, sym_hand, handBasisInfo.Count(), handBasisInfo.Atoms());
	    }

	    if(hands_confidence_flag){
		SetFloat(handInfo[0], i);
		SetSymbol(handInfo[1], sym_confidence);
		SetFloat(handInfo[2], hand.confidence());
		ToOutAnything(1, sym_hand, 3, handInfo.Atoms());
	    }

	    if(hands_is_right_flag){
		SetFloat(handInfo[0], i);
		SetSymbol(handInfo[1], sym_is_right);
		SetFloat(handInfo[2], hand.isRight());
		ToOutAnything(1, sym_hand, 3, handInfo.Atoms());
	    }

	    if(hands_grab_strength_flag){
		SetFloat(handInfo[0], i);
		SetSymbol(handInfo[1], sym_grab_strength);
		SetFloat(handInfo[2], hand.grabStrength());
		ToOutAnything(1, sym_hand, 3, handInfo.Atoms());
	    }

	    if(hands_pinch_strength_flag){
		SetFloat(handInfo[0], i);
		SetSymbol(handInfo[1], sym_pinch_strength);
		SetFloat(handInfo[2], hand.pinchStrength());
		ToOutAnything(1, sym_hand, 3, handInfo.Atoms());
	    }
			 
	    if(hands_arm_flag){
		AtomList handArmInfo(11);
		Arm arm = hand.arm();

		SetFloat(handArmInfo[0], i);
		SetSymbol(handArmInfo[1], sym_arm);
		SetFloat(handArmInfo[2], arm.elbowPosition().x); 
		SetFloat(handArmInfo[3], arm.elbowPosition().y); 
		SetFloat(handArmInfo[4], arm.elbowPosition().z); 
		SetFloat(handArmInfo[5], arm.wristPosition().x); 
		SetFloat(handArmInfo[6], arm.wristPosition().y); 
		SetFloat(handArmInfo[7], arm.wristPosition().z); 
		SetFloat(handArmInfo[8], arm.direction().x); 	    
		SetFloat(handArmInfo[9], arm.direction().y); 	    
		SetFloat(handArmInfo[10], arm.direction().z); 	    
		ToOutAnything(1, sym_hand, handArmInfo.Count(), handArmInfo.Atoms());
	    }

            for(int j = 0; j<num_fingers; j++){
                Finger finger;
                finger = hand.fingers()[j];                    
                AtomList fingerInfo(7);
                if(fingers_direction_flag){
                    SetFloat(fingerInfo[0], i); // index
                    SetSymbol(fingerInfo[1], sym_fingers);
                    SetFloat(fingerInfo[2], j);
                    SetSymbol(fingerInfo[3], sym_direction);
                    SetFloat(fingerInfo[4], finger.direction().x);
                    SetFloat(fingerInfo[5], finger.direction().y);
                    SetFloat(fingerInfo[6], finger.direction().z);
                    ToOutAnything(1, sym_hand, fingerInfo.Count(), fingerInfo.Atoms());
                }
                if(fingers_position_flag){
                    SetFloat(fingerInfo[0], i); // index
                    SetSymbol(fingerInfo[1], sym_fingers);
                    SetFloat(fingerInfo[2], j);
                    SetSymbol(fingerInfo[3], sym_position);
                    SetFloat(fingerInfo[4], finger.tipPosition().x);
                    SetFloat(fingerInfo[5], finger.tipPosition().y);
                    SetFloat(fingerInfo[6], finger.tipPosition().z);
                    ToOutAnything(1, sym_hand, fingerInfo.Count(), fingerInfo.Atoms());
                }
                if(fingers_velocity_flag){
                    SetFloat(fingerInfo[0], i); // index
                    SetSymbol(fingerInfo[1], sym_fingers);
                    SetFloat(fingerInfo[2], j);
                    SetSymbol(fingerInfo[3], sym_velocity);
                    SetFloat(fingerInfo[4], finger.tipVelocity().x);
                    SetFloat(fingerInfo[5], finger.tipVelocity().y);
                    SetFloat(fingerInfo[6], finger.tipVelocity().z);
                    ToOutAnything(1, sym_hand, fingerInfo.Count(), fingerInfo.Atoms());
                }
                if(fingers_size_flag){
                    SetFloat(fingerInfo[0], i); // index
                    SetSymbol(fingerInfo[1], sym_fingers);
                    SetFloat(fingerInfo[2], j);
                    SetSymbol(fingerInfo[3], sym_size);
                    SetFloat(fingerInfo[4], finger.width());
                    SetFloat(fingerInfo[5], finger.length());
                    ToOutAnything(1, sym_hand, 6, fingerInfo.Atoms());
                }
            }
        }
	//AtomList gestureCountInfo(1);            
	for(int i = 0;i < num_gestures; i++){
	    Gesture gesture;
	    gesture = frame.gestures()[i];
            //type
            AtomList gestureTypeInfo(3);
            SetFloat(gestureTypeInfo[0], i);
            SetSymbol(gestureTypeInfo[1], sym_type);
            switch(gesture.type())
            {
                case Gesture::TYPE_INVALID:
                    SetSymbol(gestureTypeInfo[2], sym_TYPE_INVALID);
                    break;
                case Gesture::TYPE_SWIPE:
                    SetSymbol(gestureTypeInfo[2], sym_TYPE_SWIPE);
                    break;
                case Gesture::TYPE_CIRCLE:
                    SetSymbol(gestureTypeInfo[2], sym_TYPE_CIRCLE);
                    break;
                case Gesture::TYPE_SCREEN_TAP:
                    SetSymbol(gestureTypeInfo[2], sym_TYPE_SCREEN_TAP);
                    break;
                case Gesture::TYPE_KEY_TAP:
                    SetSymbol(gestureTypeInfo[2], sym_TYPE_KEY_TAP);
                    break;
            }
            ToOutList(2, gestureTypeInfo);

            //state
            AtomList gestureStateInfo(3);
            SetFloat(gestureStateInfo[0], i);
            SetSymbol(gestureStateInfo[1], sym_state);
            switch(gesture.state())
            {
                case Gesture::STATE_INVALID:
                    SetSymbol(gestureStateInfo[2], sym_STATE_INVALID);
                    break;
                case Gesture::STATE_START:
                    SetSymbol(gestureStateInfo[2], sym_STATE_START);
                    break;
                case Gesture::STATE_UPDATE:
                    SetSymbol(gestureStateInfo[2], sym_STATE_UPDATE);
                    break;
                case Gesture::STATE_STOP:
                    SetSymbol(gestureStateInfo[2], sym_STATE_STOP);
                    break;
            }
            ToOutList(2, gestureStateInfo);

            AtomList gestureDurationInfo(3);
            SetFloat(gestureDurationInfo[0], i);
            SetSymbol(gestureDurationInfo[1], sym_duration);
            SetFloat(gestureDurationInfo[2], gesture.duration());
            ToOutList(2, gestureDurationInfo);

            AtomList gestureIdInfo(3);
            SetFloat(gestureIdInfo[0], i);
            SetSymbol(gestureIdInfo[1], sym_id);
            SetFloat(gestureIdInfo[2], gesture.id());
            ToOutList(2, gestureIdInfo);

        }
    }
    void m_general(int s)
    {
        general_flag = s;
    }
    void m_hands_direction(int s)
    {
        hands_direction_flag = s;
    }
    void m_hands_palm_position(int s)
    {
        hands_palm_position_flag = s;
    }
    void m_hands_palm_velocity(int s)
    {
        hands_palm_velocity_flag = s;
    }
    void m_hands_palm_normal(int s)
    {
        hands_palm_normal_flag = s;
    }
    void m_hands_finger_count(int s)
    {
        hands_finger_count_flag = s;
    }
    void m_hands_tool_count(int s)
    {
        hands_tool_count_flag = s;
    }
    void m_hands_basis(int s)
    {
	hands_basis_flag = s;
    }
    void m_hands_confidence(int s)
    {
	hands_confidence_flag = s;
    }
    void m_hands_is_right(int s)
    {
	hands_is_right_flag = s;
    }
    void m_hands_grab_strength(int s)
    {
	hands_grab_strength_flag = s;
    }
    void m_hands_pinch_strength(int s)
    {
	hands_pinch_strength_flag = s;
    }
    void m_hands_arm(int s)
    {
	hands_arm_flag = s;
    }
    void m_hands_sphere_radius(int s)
    {
        hands_sphere_radius_flag = s;
    }
    void m_hands_sphere_center(int s)
    {
        hands_sphere_center_flag = s;
    }
    void m_tools_direction(int s)
    {
        tools_direction_flag = s;
    }
    void m_tools_position(int s)
    {
        tools_position_flag = s;
    }
    void m_tools_velocity(int s)
    {
        tools_velocity_flag = s;
    }
    void m_tools_size(int s)
    {
        tools_size_flag = s;
    }
    void m_fingers_direction(int s)
    {
        fingers_direction_flag = s;
    }
    void m_fingers_position(int s)
    {
        fingers_position_flag = s;
    }
    void m_fingers_velocity(int s)
    {
        fingers_velocity_flag = s;
    }
    void m_fingers_size(int s)
    {
        fingers_size_flag = s;
    }
    void m_gestures(int argc, const t_atom *argv)
    {
        if(argc > 2){
            post("leapmotion: gestures too many arguments");
        }
        else if(argc < 2){
            post("leapmotion: gestures too few arguments");
        }
        else{
            if((argv[0].a_type != A_SYMBOL) || (argv[1].a_type != A_FLOAT)){
                post("leapmotion: gestures wrong arguments");
                return;
            }
            char *type = argv[0].a_w.w_symbol->s_name;
            float s = argv[1].a_w.w_float;
            
            if(strcmp(type, "TYPE_CIRCLE") == 0){
                if(s == 0.0){
                    controller.enableGesture(Gesture::TYPE_CIRCLE, false);
                }else{
                    controller.enableGesture(Gesture::TYPE_CIRCLE, true);
                }
            }
            if(strcmp(type, "TYPE_SWIPE") == 0){
                if(s == 0.0){
                    controller.enableGesture(Gesture::TYPE_SWIPE, false);
                }else{
                    controller.enableGesture(Gesture::TYPE_SWIPE, true);
                }
            }
            if(strcmp(type, "TYPE_KEY_TAP") == 0){
                if(s == 0.0){
                    controller.enableGesture(Gesture::TYPE_KEY_TAP, false);
                }else{
                    controller.enableGesture(Gesture::TYPE_KEY_TAP, true);
                }
            }
            if(strcmp(type, "TYPE_SCREEN_TAP") == 0){
                if(s == 0.0){
                    controller.enableGesture(Gesture::TYPE_SCREEN_TAP, false);
                }else{
                    controller.enableGesture(Gesture::TYPE_SCREEN_TAP, true);
                }
            }
        }
    }
    void m_info()
    {
        post("-LEAP SETTING-");
        post("General Info:%s", flag_to_string(general_flag));
        
        post("-Hands-");
        post("direction:%s", flag_to_string(hands_direction_flag));
        post("palm position:%s", flag_to_string(hands_palm_position_flag));
        post("palm velocity:%s", flag_to_string(hands_palm_velocity_flag));
        post("palm normal:%s", flag_to_string(hands_palm_normal_flag));

        post("finger count:%s", flag_to_string(hands_finger_count_flag));
        post("tool count:%s", flag_to_string(hands_tool_count_flag));
        
	post("confidence:%s", flag_to_string(hands_confidence_flag));
	post("is_right:%s", flag_to_string(hands_is_right_flag));
	post("hands basis:%s", flag_to_string(hands_basis_flag));
	post("arm:%s", flag_to_string(hands_arm_flag));
	
	post("sphere radius:%s", flag_to_string(hands_sphere_radius_flag));
        post("sphere center:%s", flag_to_string(hands_sphere_center_flag));
        
        post("-Tools-");
        post("direction:%s", flag_to_string(tools_direction_flag));
        post("position:%s", flag_to_string(tools_position_flag));
        post("velocity:%s", flag_to_string(tools_velocity_flag));
        post("size:%s", flag_to_string(tools_size_flag));
        
        post("-Fingers-");
        post("direction:%s", flag_to_string(fingers_direction_flag));
        post("position:%s", flag_to_string(fingers_position_flag));
        post("velocity:%s", flag_to_string(fingers_velocity_flag));
        post("size:%s", flag_to_string(fingers_size_flag));
        
        post("-Gestures-");
        post("TYPE_CIRCLE:%s", flag_to_string(controller.isGestureEnabled(Gesture::TYPE_CIRCLE)));
        post("TYPE_SWIPE:%s", flag_to_string(controller.isGestureEnabled(Gesture::TYPE_SWIPE)));
        post("TYPE_KEY_TAP:%s", flag_to_string(controller.isGestureEnabled(Gesture::TYPE_KEY_TAP)));
        post("TYPE_SCREEN_TAP:%s", flag_to_string(controller.isGestureEnabled(Gesture::TYPE_SCREEN_TAP)));

    }
private:
    
    const char* flag_to_string(int flag)
    {
        if(flag){
            return "on";
        }else{
            return "off";
        }
    }
    FLEXT_CALLBACK(m_info);
    FLEXT_CALLBACK(m_bang);
    FLEXT_CALLBACK_I(m_general);
    FLEXT_CALLBACK_I(m_hands_direction);
    FLEXT_CALLBACK_I(m_hands_palm_position);
    FLEXT_CALLBACK_I(m_hands_palm_velocity);
    FLEXT_CALLBACK_I(m_hands_palm_normal);

    FLEXT_CALLBACK_I(m_hands_sphere_radius);
    FLEXT_CALLBACK_I(m_hands_sphere_center);
    FLEXT_CALLBACK_I(m_hands_finger_count);
    FLEXT_CALLBACK_I(m_hands_tool_count);

    FLEXT_CALLBACK_I(m_hands_basis);
    FLEXT_CALLBACK_I(m_hands_confidence);
    FLEXT_CALLBACK_I(m_hands_is_right);
    FLEXT_CALLBACK_I(m_hands_grab_strength);
    FLEXT_CALLBACK_I(m_hands_pinch_strength);
    FLEXT_CALLBACK_I(m_hands_arm);

    FLEXT_CALLBACK_I(m_tools_direction);
    FLEXT_CALLBACK_I(m_tools_position);
    FLEXT_CALLBACK_I(m_tools_velocity);
    FLEXT_CALLBACK_I(m_tools_size);
    FLEXT_CALLBACK_I(m_fingers_direction);
    FLEXT_CALLBACK_I(m_fingers_position);
    FLEXT_CALLBACK_I(m_fingers_velocity);
    FLEXT_CALLBACK_I(m_fingers_size);
    FLEXT_CALLBACK_V(m_gestures);
};

FLEXT_NEW("leapmotion",leapmotion)

