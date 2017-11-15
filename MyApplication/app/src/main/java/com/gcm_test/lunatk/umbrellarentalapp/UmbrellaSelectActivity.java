package com.gcm_test.lunatk.umbrellarentalapp;

import android.content.Intent;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.util.TypedValue;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

/**
 * Created by LunaTK on 2017. 11. 15..
 */

public class UmbrellaSelectActivity extends AppCompatActivity {

    private TableLayout tl;
    private Boolean umbrellaState[][];
    private int row=10, column=5;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_umbrella_select);
        tl  = (TableLayout)findViewById(R.id.table_layout);
        updateUmbrellaState();
        tl.getViewTreeObserver()
                .addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
                    @Override
                    public void onGlobalLayout() {


                        buildTable(row,column);
//                        fillCountryTable();
                        //리스너 해제
                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
                            tl.getViewTreeObserver().removeOnGlobalLayoutListener(this);
                        }
                    }
                });
    }

    private void updateUmbrellaState(){
        umbrellaState = new Boolean[row][column];
        int k = 1;
        for(int i = 0;i<row;i++)
            for(int j = 0;j<column;j++)
                if((k++)%7==0) umbrellaState[i][j]=false;
                else umbrellaState[i][j] = true;
    }

    private void buildTable(int row, int column) {
        TableRow rows[] = new TableRow[row];
        OnClickListener listener = new OnClickListener();
        int number=1;
        for(int i = 0;i<row;i++){
            rows[i] = new TableRow(this);

            Button buttons[] = new Button[column];
            for(int j = 0;j<column;j++){
                Button btn = new Button(this);
                buttons[j] = btn;
                btn.setEnabled(umbrellaState[i][j]);
                if(!umbrellaState[i][j]){
                    btn.setTextColor(getResources().getColor(android.R.color.holo_red_light));
                } else {
                    btn.setTextColor(getResources().getColor(android.R.color.holo_green_light));
                }
                btn.setOnClickListener(listener);
                btn.setText(number++ +"");
                btn.setTextSize(8);
                btn.setTextColor(Color.BLACK);
                btn.setTypeface(null,1);
                btn.setWidth(10);
                btn.setHeight(TableLayout.LayoutParams.WRAP_CONTENT);
                btn.setPadding(10,10,10,10);
                rows[i].addView(btn);
            }
            tl.addView(rows[i],new TableLayout.LayoutParams(TableLayout.LayoutParams.WRAP_CONTENT,TableLayout.LayoutParams.WRAP_CONTENT));
        }

    }

    class OnClickListener implements View.OnClickListener {

        @Override
        public void onClick(View view) {
            Intent intent = new Intent(UmbrellaSelectActivity.this, QRPopupActivity.class);
            startActivity(intent);
        }
    }
}
