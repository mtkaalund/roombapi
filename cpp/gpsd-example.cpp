//  Example C++17 gpsd program using libgpsmm

//  Has no actual purpose other than output some of libgpsmm struct variables of your choosing.
//  reference declarations: https://fossies.org/dox/gpsd-3.22/index.html
//
//  compiles without warning as follows:
//  g++ -std=c++17 -Wall -pedantic -pthread $(pkg-config --cflags --libs libgps) gpsd-example.cpp -o gpsd-example
//  clang++ -std=c++17 -Wall -pedantic -pthread $(pkg-config --cflags --libs libgps) gpsd-example.cpp -o gpsd-example
//  Free for the taking.
//  Version 1.84

#include <libgpsmm.h>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

#include <array>

enum TimeFormat { LOCALTIME, UTC, UNIX, ISO_8601 };

auto TimespecToTimeStr(const timespec& gpsd_time, TimeFormat time_format = LOCALTIME) {
  // example showing different ways of formating GPSD's timespec, depending on requirement
  std::ostringstream oss;
  switch (time_format) {
    case LOCALTIME: {
      // convert timespec_t into localtime (dd-mm-YY HH:MM:SS)
      const auto tm = *std::localtime(&gpsd_time.tv_sec);
      oss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
      break;
    }
    case UTC: {
      // convert timespec_t into UTC (dd-mm-YY HH:MM:SS)
      const auto tm = *std::gmtime(&gpsd_time.tv_sec);
      oss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
      break;
    }
    case UNIX:
      // returns seconds since the Epoch
      oss << gpsd_time.tv_sec;
      break;
    case ISO_8601: {
      // convert timespec_t into ISO8601 UTC time (yyyy-MM-dd'T'HH:mm:ss'Z')
      constexpr size_t kScrSize{128};
      std::array<char, kScrSize> scr{};
      timespec_to_iso8601(gpsd_time, scr.data(), kScrSize);
      oss << scr.data();
      break;
    }
  }
  return oss.str();
}

auto main() -> int {


  gpsmm gps_rec("localhost", DEFAULT_GPSD_PORT);

  if (gps_rec.stream(WATCH_ENABLE | WATCH_JSON) == nullptr) {
    std::cerr << "No GPSD running.\n";
    return 1;
  }

  constexpr auto kWaitingTime{1000000};  // 1000000 is 1 second

  for (;;) {
    if (!gps_rec.waiting(kWaitingTime)) {
      continue;
    }

    struct gps_data_t* gpsd_data;

    if ((gps_rec.read()) == nullptr) {
      std::cerr << "GPSD read error.\n";
      return 1;
    }

    while (((gpsd_data = gps_rec.read()) == nullptr) || (gpsd_data->fix.mode < MODE_2D)) {
      // Do nothing until fix, block execution for 1 second (busy wait mitigation)
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    const auto get_dop_meaning = [&](float v) {
      std::string str;
      if(v < 1.0f) {
        str = "Ideal";
      } else if(v >= 1 && v < 2) {
        str = "Excellent";
      } else if(v >= 2 && v < 5) {
        str = "Good";
      } else if(v >= 5 && v < 10 ) {
        str = "Moderate";
      } else if(v >= 10 && v < 20) {
        str = "Fair";
      } else if(v >= 20) {
        str = "Poor";
      }

      return str;
    };

    const auto latitude{gpsd_data->fix.latitude};
    const auto longitude{gpsd_data->fix.longitude};
    const auto speed{gpsd_data->fix.speed};
    const auto hdop{gpsd_data->dop.hdop};
    const auto vdop{gpsd_data->dop.vdop};
    const auto pdop{gpsd_data->dop.pdop};
    const auto s_vis{gpsd_data->satellites_visible};
    const auto s_used{gpsd_data->satellites_used};
    const auto time_str{TimespecToTimeStr(gpsd_data->fix.time, UTC)};  // you can change the 2nd argument to LOCALTIME, UTC, UNIX or ISO8601

    std::cout << std::setprecision(8) << std::fixed;  // set output to fixed floating point, 8 decimal precision
    std::cout << time_str << "\n\tLatitude: " << latitude << " Longitude: " << longitude << " Speed: " << speed << "\n";
    std::cout << "\tHDOP: " << hdop << " " << get_dop_meaning(hdop) << " VDOP: " << vdop << " " << get_dop_meaning(vdop) << " PDOP: " << pdop << " " << get_dop_meaning(pdop) << "\n";
    std::cout << "\tSatellites visible: " << s_vis << " Satellites used: " << s_used << "\n";
    for(int i = 0; i < s_vis; i++)
    {
      std::string sat_id = "";
      switch (gpsd_data->skyview[i].gnssid)
      {
      case GNSSID_GPS:
        sat_id = "GPS";
        break;
      case GNSSID_SBAS:
        sat_id = "SBAS";
        break;
      case GNSSID_GAL:
        sat_id = "Galileo";
        break;
      case GNSSID_BD:
        sat_id = "BeiDou";
        break;
      case GNSSID_IMES:
        sat_id = "IMES";
        break;
      case GNSSID_QZSS:
        sat_id = "QZSS";
        break;
      case GNSSID_GLO:
        sat_id = "GLONASS";
        break;
      case GNSSID_IRNSS:
        sat_id = "NavIC";
        break;
      default:
        break;
      }

      std::string sat_str = "";
      switch (gpsd_data->skyview[i].health)
      {
      case SAT_HEALTH_UNK:
        sat_str = "Unknown";
        break;

      case SAT_HEALTH_OK:
        sat_str = "Okay";
        break;
      case SAT_HEALTH_BAD:
        sat_str = "Bad";
        break;
      
      default:
        break;
      }


      std::cout << "\t\t" << gpsd_data->skyview[i].PRN << ": "
                << gpsd_data->skyview[i].elevation << " "
                << gpsd_data->skyview[i].azimuth << " "
                << gpsd_data->skyview[i].ss << " "
                << sat_str << " "
                << (int)gpsd_data->skyview[i].freqid << " "
                << sat_id << " "
                << (gpsd_data->skyview[i].used ? "Y" : "N") << std::endl;
    }
  }
}
