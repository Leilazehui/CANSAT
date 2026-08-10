// =====================================================
// ESP32-CAM COMMUNICATION
// =====================================================

// Nano receives ESP32 commands on D2
// Nano D7 is reserved for possible future TX

SoftwareSerial ESP32Serial(2, 7);

// =====================================================
// CAMERA FALLBACK
// =====================================================

bool fallbackMode = false;

unsigned long fallbackStartTime = 0;

const unsigned long CAMERA_TIMEOUT = 1000;

// Approximate time required to travel 5 m
const unsigned long FALLBACK_TIME = 5000;

const byte FALLBACK_SPEED = 150;

// =====================================================
// MISSION STATES
// =====================================================

enum MissionState
{
    ASCENT,
    DESCENT,
    LANDING,
    GROUND_EGRESS
};


MissionState missionState = ASCENT;


// =====================================================
// ESP32 COMMAND TIMEOUT
// =====================================================

unsigned long lastESP32Command = 0;

const unsigned long ESP32_TIMEOUT = 2000;

// =====================================================
// LANDING DETECTION
// =====================================================

// =====================================================
// LANDING DETECTION
// =====================================================

unsigned long stableTime = 0;

const float LANDING_VERTICAL_SPEED = 0.5;  // m/s

// Maximum altitude change allowed during
// the landing stability period
const float LANDING_ALTITUDE_CHANGE = 0.5; // metres

// Required stable time
const unsigned long LANDING_STABLE_TIME = 3000; // ms

// Store altitude when stability begins
float landingReferenceAltitude = 0;

// =====================================================
// SETUP
// =====================================================

void setup()
{
    Serial.begin(9600);

    ESP32Serial.begin(9600);


    Serial.println("==============================");
    Serial.println("CANSAT INITIALISING");
    Serial.println("==============================");


    // -------------------------------------------------
    // Sensors
    // -------------------------------------------------

    initSensors();

    initAltitude();


    // -------------------------------------------------
    // Telemetry
    // -------------------------------------------------

    initTelemetry();


    // -------------------------------------------------
    // Motors
    // -------------------------------------------------

    initMotors();


    // -------------------------------------------------
    // Parachute servo
    // -------------------------------------------------

    initParachute();


    // -------------------------------------------------
    // Initial condition
    // -------------------------------------------------

    stopMotors();


    Serial.println("CANSAT SYSTEM READY");
}


// =====================================================
// LANDING DETECTION
// =====================================================

bool landingDetected()
{
    float verticalSpeed = getVerticalSpeed();
    float currentAltitude = getCurrentAltitude();


    // =================================================
    // CONDITION 1:
    // Vertical speed must be close to zero
    // =================================================

    bool lowVerticalSpeed =
        abs(verticalSpeed) < LANDING_VERTICAL_SPEED;


    // =================================================
    // CONDITION 2:
    // Altitude must remain approximately constant
    // =================================================

    bool stableAltitude = false;


    if (stableTime != 0)
    {
        float altitudeChange =
            abs(currentAltitude -
                landingReferenceAltitude);

        stableAltitude =
            altitudeChange < LANDING_ALTITUDE_CHANGE;
    }


    // =================================================
    // BOTH CONDITIONS MUST BE TRUE
    // =================================================

    if (lowVerticalSpeed)
    {

        // ---------------------------------------------
        // Start stability timer
        // ---------------------------------------------

        if (stableTime == 0)
        {
            stableTime = millis();

            landingReferenceAltitude =
                currentAltitude;

            Serial.println(
                "Possible landing detected..."
            );
        }


        // ---------------------------------------------
        // Check altitude stability
        // ---------------------------------------------

        float altitudeChange =
            abs(currentAltitude -
                landingReferenceAltitude);

        stableAltitude =
            altitudeChange <
            LANDING_ALTITUDE_CHANGE;


        // ---------------------------------------------
        // Require both conditions for 3 seconds
        // ---------------------------------------------

        if (stableAltitude &&
            millis() - stableTime >=
            LANDING_STABLE_TIME)
        {
            Serial.println(
                "LANDING CONFIRMED"
            );

            return true;
        }
    }

    else
    {
        // ---------------------------------------------
        // CanSat is still moving vertically
        // Reset stability timer
        // ---------------------------------------------

        stableTime = 0;

        Serial.println(
            "Landing stability lost"
        );
    }


    return false;
}

// =====================================================
// CAMERA FALLBACK MOTION
// =====================================================

void startFallback()
{
    if (fallbackMode)
        return;


    fallbackMode = true;

    fallbackStartTime = millis();


    Serial.println(
        "WARNING: ESP32 CAMERA LOST"
    );

    Serial.println(
        "ENTERING 5 m FALLBACK MODE"
    );


    moveForward(FALLBACK_SPEED);
}


// =====================================================
// UPDATE FALLBACK
// =====================================================

void updateFallback()
{
    if (!fallbackMode)
        return;


    unsigned long elapsed =
        millis() - fallbackStartTime;


    if (elapsed >= FALLBACK_TIME)
    {
        stopMotors();

        fallbackMode = false;

        Serial.println(
            "FALLBACK COMPLETE"
        );

        Serial.println(
            "MOTORS STOPPED"
        );
    }
}

// =====================================================
// ESP32 COMMAND PROCESSING
// =====================================================

void processESP32Command()
{
    // =================================================
    // ONLY RUN DURING GROUND EGRESS
    // =================================================

    if (missionState != GROUND_EGRESS)
    {
        stopMotors();
        return;
    }


    // =================================================
    // IF FALLBACK IS ACTIVE
    // =================================================

    if (fallbackMode)
    {
        updateFallback();
        return;
    }


    // =================================================
    // RECEIVE ESP32 COMMAND
    // =================================================

    if (ESP32Serial.available())
    {
        char command =
            ESP32Serial.read();


        lastESP32Command =
            millis();


        Serial.print("ESP32 COMMAND: ");
        Serial.println(command);


        switch (command)
        {

            case 'F':

                moveForward(150);

                break;


            case 'L':

                turnLeft(150);

                break;


            case 'R':

                turnRight(150);

                break;


            case 'S':

                stopMotors();

                break;


            default:

                stopMotors();

                break;
        }
    }


    // =================================================
    // CAMERA COMMUNICATION TIMEOUT
    // =================================================

    if (millis() - lastESP32Command >
        CAMERA_TIMEOUT)
    {
        startFallback();
    }
}


// =====================================================
// LOOP
// =====================================================

void loop()
{
    // =================================================
    // UPDATE SENSORS
    // =================================================

    updateSensors();


    // =================================================
    // UPDATE ALTITUDE
    // =================================================

    updateAltitude();


    // =================================================
    // SEND TELEMETRY
    // =================================================

    sendTelemetry();


    // =================================================
    // MISSION STATE MACHINE
    // =================================================

    switch (missionState)
    {

        // =============================================
        // ASCENT
        // =============================================

        case ASCENT:

            stopMotors();


            Serial.println("STATE: ASCENT");


            // If vertical speed becomes negative,
            // CanSat has reached descent phase.

            if (getVerticalSpeed() < -0.5)
            {
                missionState = DESCENT;

                Serial.println("STATE CHANGE: DESCENT");
            }

            break;


        // =============================================
        // DESCENT
        // =============================================

        case DESCENT:

            stopMotors();


            Serial.println("STATE: DESCENT");


            if (landingDetected())
            {
                missionState = LANDING;

                Serial.println(
                    "STATE CHANGE: LANDING"
                );
            }

            break;


        // =============================================
        // LANDING
        // =============================================

        case LANDING:

            stopMotors();


            Serial.println("STATE: LANDING");


            // -----------------------------------------
            // Release parachute
            // -----------------------------------------

            releaseParachute();


            delay(1500);


            // -----------------------------------------
            // Enable ground egress
            // -----------------------------------------

            missionState =
                GROUND_EGRESS;


            // Reset ESP32 watchdog timer

            lastESP32Command =
                millis();


            Serial.println(
                "STATE CHANGE: GROUND EGRESS"
            );

            break;


        // =============================================
        // GROUND EGRESS
        // =============================================

        case GROUND_EGRESS:

            Serial.println(
                "STATE: GROUND EGRESS"
            );


            // ESP32 determines navigation.
            //
            // ESP32
            //    ↓
            // UART
            //    ↓
            // Nano
            //    ↓
            // L298N
            //    ↓
            // Motors

            processESP32Command();

            break;
    }


    delay(100);
}
