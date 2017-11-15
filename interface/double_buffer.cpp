#include <mutex>


class Double_buffer
{
 private:
  std::mutex first_buffer;
  std::mutex second_buffer;
 public:
  bool add_chunk(std::string  );
  void start_processing();
  void * get_result();
  Double_buffer();
  virtual ~Double_buffer();
};
