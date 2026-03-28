"use client";

import {
  CartesianGrid,
  Line,
  LineChart,
  Tooltip,
  XAxis,
  YAxis,
} from "recharts";

type ChartPoint = {
  date: string;
  close: number;
};

export default function StockChart({ data }: { data: ChartPoint[] }) {
  if (!data.length) {
    return <p>No chart data.</p>;
  }

  return (
    <section>
      <h3>Close Price</h3>
      <LineChart width={900} height={320} data={data}>
        <CartesianGrid strokeDasharray="3 3" />
        <XAxis dataKey="date" />
        <YAxis />
        <Tooltip />
        <Line type="monotone" dataKey="close" stroke="#000" dot={false} />
      </LineChart>
    </section>
  );
}
