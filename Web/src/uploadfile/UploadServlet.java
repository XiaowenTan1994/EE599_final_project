package uploadfile;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.nio.file.Paths;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.annotation.MultipartConfig;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.Part;

import org.apache.tomcat.util.http.fileupload.FileItem;
import org.apache.tomcat.util.http.fileupload.RequestContext;
import org.apache.tomcat.util.http.fileupload.disk.DiskFileItemFactory;
import org.apache.tomcat.util.http.fileupload.servlet.ServletFileUpload;

/**
 * Servlet implementation class UploadServlet
 */
@WebServlet("/UploadServlet")
@MultipartConfig
public class UploadServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
	private static final int MEMORY_THRESHOLD = 1024*1024*15;
	private static final long MAX_FILE_SIZE = 1024 * 1024 * 40;
	private static final long MAX_REQUEST_SIZE = 1024 * 1024 * 50;
	private static final String UPLOAD_DIRECTORY = "D:\\Users\\TXWSD\\eclipse-laioffer-workspace\\TextRecognitionWeb\\download";
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public UploadServlet() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		response.getWriter().append("Served at: ").append(request.getContextPath());
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		 if (!ServletFileUpload.isMultipartContent(request)) {
	            // 如果不是则停止
	            PrintWriter writer = response.getWriter();
	            writer.println("Error: 表单必须包含 enctype=multipart/form-data");
	            writer.flush();
	            return;
	        }
		 Part filepart = request.getPart("uploadfile");
		 String filename = "input.png";
		 InputStream content = filepart.getInputStream();
		 int a=0;
	     byte[] data = null;
			try {
				data = readInputStream(content);
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}  
	        File imageFile = new File(filename);  
	        FileOutputStream outStream = new FileOutputStream(imageFile);    
	        outStream.write(data);  
	        outStream.close();
	        response.setContentType("text/html");
	        PrintWriter writer=response.getWriter();
	        Process proc;
	        try {
	        	proc = Runtime.getRuntime().exec("python demo.py input.png");
	        	BufferedReader in = new BufferedReader(new InputStreamReader(proc.getInputStream()));
	        	String output = null;
				while ((output= in.readLine()) != null) {
					writer.write(output);
				}
	        }catch (IOException e) {
	        	e.printStackTrace();
	        } 
	}
	
    public static byte[] readInputStream(InputStream inStream) throws Exception{  
        ByteArrayOutputStream outStream = new ByteArrayOutputStream();  
        //创建一个Buffer字符串  
        byte[] buffer = new byte[1024];  
        //每次读取的字符串长度，如果为-1，代表全部读取完毕  
        int len = 0;  
        //使用一个输入流从buffer里把数据读取出来  
        while( (len=inStream.read(buffer)) != -1 ){  
            //用输出流往buffer里写入数据，中间参数代表从哪个位置开始读，len代表读取的长度  
            outStream.write(buffer, 0, len);  
        }  
        //关闭输入流  
        inStream.close();  
        //把outStream里的数据写入内存  
        return outStream.toByteArray();  
    }  

}
