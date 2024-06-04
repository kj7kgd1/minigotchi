/**
 * channel.cpp: handles channel switching
*/

#include "channel.h"

/** developer note:
 *
 * i am using the ideal 2.4 GHz channels, which are 1, 6, and 11.
 * the reason for that is so we don't interfere with other devices on our frequency.
 * there are probably plenty more reasons but this is a good practice for such iot devices.
 *
 */

// same channels in config
int Channel::channelList[13] = {
    Config::channels[0], Config::channels[1], Config::channels[2], Config::channels[3],
    Config::channels[4], Config::channels[5], Config::channels[6], Config::channels[7],
    Config::channels[8], Config::channels[9], Config::channels[10], Config::channels[11],
    Config::channels[12]
};

void Channel::init(int initChannel) {
    // start on user specified channel
    delay(250);
    Serial.println(" ");
    Serial.print("(-.-) Initializing on channel ");
    Serial.println(initChannel);
    Serial.println(" ");
    Display::cleanDisplayFace("(-.-)");
    Display::attachSmallText("Initializing on channel " + (String) initChannel);
    delay(250);

    // switch channel
    Minigotchi::monStop();
    wifi_set_channel(initChannel);
    Minigotchi::monStart();

    if (initChannel == getChannel()) {
        Serial.print("('-') Successfully initialized on channel ");
        Serial.println(getChannel());
        Display::cleanDisplayFace("('-')");
        Display::attachSmallText("Successfully initialized on channel " + (String) getChannel());
        delay(250);
    } else {
        Serial.print("(X-X) Channel initialization failed, try again?");
        Display::cleanDisplayFace("(X-X)");
        Display::attachSmallText("Channel initialization failed, try again?");
        delay(250);
    }
}

void Channel::cycle() {
    // get channels
    int numChannels = sizeof(channelList) / sizeof(channelList[0]);

    // select a random one
    int randomIndex = random(numChannels);
    int newChannel = channelList[randomIndex];

    // switch here
    switchChannel(newChannel);
}

void Channel::switchChannel(int newChannel) {
    // switch to channel
    delay(250);
    Serial.print("(-.-) Switching to channel ");
    Serial.println(newChannel);
    Serial.println(" ");
    Display::cleanDisplayFace("(-.-)");
    Display::attachSmallText("Switching to channel " + (String) newChannel);
    delay(250);

    // monitor this one channel
    Minigotchi::monStop();
    wifi_set_channel(newChannel);
    Minigotchi::monStart();

    // switched channel
    checkChannel(newChannel);
}

// check if the channel switch was successful
void Channel::checkChannel(int channel) {
    int currentChannel = Channel::getChannel();
    if (channel == getChannel()) {
        Serial.print("('-') Currently on channel ");
        Serial.println(currentChannel);
        Display::cleanDisplayFace("('-')");
        Display::attachSmallText("Currently on channel " + (String) currentChannel);
        Serial.println(" ");
        delay(250);
    } else {
        Serial.print("(X-X) Channel switch to channel ");
        Serial.print(channel);
        Serial.println(" has failed");
        Serial.print("(X-X) Currently on channel ");
        Serial.print(currentChannel);
        Serial.println(" instead");
        Serial.println(" ");
        Display::cleanDisplayFace("(X-X)");
        Display::attachSmallText("Channel switch to " + (String) channel + " has failed");
        delay(250);
    }
}

bool Channel::isValidChannel(int channel) {
  bool isValidChannel = false;
  for (int i = 0; i < sizeof(channelList) / sizeof(channelList[0]); i++) {
    if (channelList[i] == channel) {
      isValidChannel = true;
      break;
    }
  }
  return isValidChannel;
}

int Channel::getChannel() {
    return wifi_get_channel();
}
