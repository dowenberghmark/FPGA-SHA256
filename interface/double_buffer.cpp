class Double_buffer
{
 public:
  bool add_chunk();
  void start_processing();
  void * get_result();
  Double_buffer();
  virtual ~Double_buffer();
};
